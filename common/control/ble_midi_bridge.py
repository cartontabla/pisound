#!/usr/bin/env python3
"""
ble_midi_bridge.py

BLE MIDI → ALSA virtual MIDI bridge.

Connects to a BLE MIDI peripheral (e.g. CME WIDI Core) via BlueZ D-Bus GATT,
subscribes to MIDI GATT notifications, strips the BLE MIDI header/timestamp
bytes, and writes raw MIDI to a snd-virmidi device so the ALSA/control chain
sees it as a normal sequencer port.

Architecture:
    BlueZ (bluetoothd) owns the BLE connection.  We communicate with it via
    the D-Bus org.bluez.Device1 and org.bluez.GattCharacteristic1 interfaces.
    BlueZ handles ATT MTU exchange, GATT service discovery, and encryption
    negotiation automatically before marking ServicesResolved=True.

    Requires a patched bluetooth.ko (skips LE_READ_REMOTE_FEATURES for
    central-role connections, and uses short connection intervals from
    main.conf [LE]) so GATT starts before the WIDI Core's connection-
    establishment timeout.

Dependencies:
    sudo apt install python3-dbus python3-gi
    sudo modprobe snd-virmidi midi_devs=1

Usage:
    sudo python3 ble_midi_bridge.py [BLE_MAC_ADDRESS]

    Default MAC: 10:2E:AB:D6:BC:4C  (WIDI Core — may change on power cycle)

BLE MIDI packet format (MIDI over BLE GATT spec):
    Byte 0  : Header    — timestamp MSB  (bit7=1, bit6=0)
    Byte 1  : Timestamp — timestamp LSB  (bit7=1)
    Bytes 2+: MIDI status + data bytes, possibly interleaved with
              more timestamp bytes before each new MIDI status byte.

    Parsing rule: a byte with bit7=1 followed immediately by another
    byte with bit7=1 is a timestamp → skip it.  Everything else is a
    real MIDI byte.
"""

import sys
import os
import time

DEFAULT_MAC        = "10:2E:AB:D6:BC:4C"
BLE_MIDI_CHAR_UUID = "7772e5db-3868-4112-a1a9-f2669d106bf3"

BLUEZ_SERVICE  = "org.bluez"
ADAPTER_IFACE  = "org.bluez.Adapter1"
DEVICE_IFACE   = "org.bluez.Device1"
GATTCHAR_IFACE = "org.bluez.GattCharacteristic1"
PROPS_IFACE    = "org.freedesktop.DBus.Properties"
OBJMGR_IFACE   = "org.freedesktop.DBus.ObjectManager"


# ---------------------------------------------------------------------------
# BLE MIDI packet parser
# ---------------------------------------------------------------------------

def parse_ble_midi(data: bytes) -> bytes:
    """Strip BLE MIDI framing and return raw MIDI bytes."""
    if len(data) < 3:
        return b''

    out = []
    i = 1  # skip header byte (index 0)

    while i < len(data):
        b = data[i]
        if b & 0x80:
            # Peek: if next byte also has bit7=1, this is a timestamp
            if i + 1 < len(data) and (data[i + 1] & 0x80):
                i += 1
                continue
            else:
                # MIDI status byte
                out.append(b)
                i += 1
        else:
            # MIDI data byte (bit7=0)
            out.append(b)
            i += 1

    return bytes(out)


# ---------------------------------------------------------------------------
# Virtual MIDI device helper
# ---------------------------------------------------------------------------

VIRMIDI_CANDIDATES = [
    "/dev/snd/midiC1D0",
    "/dev/snd/midiC2D0",
    "/dev/snd/midiC3D0",
    "/dev/snd/midiC4D0",
    "/dev/snd/midiC0D0",
]


def open_virmidi():
    """Load snd-virmidi if needed and return an open file handle."""
    os.system("modprobe snd-virmidi midi_devs=1 2>/dev/null")
    time.sleep(0.5)

    for path in VIRMIDI_CANDIDATES:
        if os.path.exists(path):
            print(f"[bridge] Virtual MIDI device: {path}")
            return open(path, 'wb', buffering=0)

    print("ERROR: no snd-virmidi device found after modprobe.", file=sys.stderr)
    print("       Run:  sudo modprobe snd-virmidi midi_devs=1", file=sys.stderr)
    sys.exit(1)


# ---------------------------------------------------------------------------
# BlueZ D-Bus GATT bridge
# ---------------------------------------------------------------------------

try:
    import dbus
    import dbus.mainloop.glib
    from gi.repository import GLib
except ImportError:
    print("ERROR: python3-dbus or python3-gi not found.", file=sys.stderr)
    print("       sudo apt install python3-dbus python3-gi", file=sys.stderr)
    sys.exit(1)


def _get_managed_objects(bus):
    om = dbus.Interface(bus.get_object(BLUEZ_SERVICE, "/"), OBJMGR_IFACE)
    return om.GetManagedObjects()


def _find_device_path(bus, mac):
    """Return the D-Bus object path for the given MAC, or None."""
    for path, ifaces in _get_managed_objects(bus).items():
        if DEVICE_IFACE in ifaces:
            addr = str(ifaces[DEVICE_IFACE].get("Address", "")).upper()
            if addr == mac.upper():
                return path
    return None


def _find_adapter_path(bus):
    for path, ifaces in _get_managed_objects(bus).items():
        if ADAPTER_IFACE in ifaces:
            return path
    return None


def _find_char_path(bus, midi_uuid):
    """Return D-Bus object path for the MIDI GATT characteristic, or None."""
    for path, ifaces in _get_managed_objects(bus).items():
        if GATTCHAR_IFACE in ifaces:
            uuid = str(ifaces[GATTCHAR_IFACE].get("UUID", "")).lower()
            if uuid == midi_uuid.lower():
                return path
    return None


def main():
    mac = sys.argv[1].upper() if len(sys.argv) > 1 else DEFAULT_MAC.upper()

    dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
    bus    = dbus.SystemBus()
    loop   = GLib.MainLoop()

    midi_fd = open_virmidi()

    # -----------------------------------------------------------------------
    # 1. Scan if device is not yet known to bluetoothd
    # -----------------------------------------------------------------------
    dev_path = _find_device_path(bus, mac)

    if not dev_path:
        print(f"[bridge] Scanning for {mac} …")
        adapter_path = _find_adapter_path(bus)
        if not adapter_path:
            print("ERROR: no Bluetooth adapter found", file=sys.stderr)
            sys.exit(1)

        adapter = dbus.Interface(
            bus.get_object(BLUEZ_SERVICE, adapter_path), ADAPTER_IFACE)
        adapter.StartDiscovery()

        found = [False]

        def _ifaces_added(path, ifaces):
            if DEVICE_IFACE in ifaces:
                addr = str(ifaces[DEVICE_IFACE].get("Address", "")).upper()
                if addr == mac:
                    found[0] = True
                    loop.quit()

        bus.add_signal_receiver(
            _ifaces_added, "InterfacesAdded", OBJMGR_IFACE,
            BLUEZ_SERVICE, "/")

        GLib.timeout_add(12000, loop.quit)
        loop.run()

        try:
            adapter.StopDiscovery()
        except Exception:
            pass

        dev_path = _find_device_path(bus, mac)
        if not dev_path:
            print(f"ERROR: {mac} not found during scan.", file=sys.stderr)
            midi_fd.close()
            sys.exit(1)

    # -----------------------------------------------------------------------
    # 2. Connect and wait for ServicesResolved
    # -----------------------------------------------------------------------
    print(f"[bridge] Connecting to {mac} via BlueZ D-Bus …")

    dev_obj   = bus.get_object(BLUEZ_SERVICE, dev_path)
    device    = dbus.Interface(dev_obj, DEVICE_IFACE)

    resolved  = [False]
    conn_loop = GLib.MainLoop()

    def _props_changed(iface, changed, _invalidated):
        if iface != DEVICE_IFACE:
            return
        if "Connected" in changed:
            print(f"[bridge] Connected={bool(changed['Connected'])}")
        if changed.get("ServicesResolved"):
            resolved[0] = True
            conn_loop.quit()

    dev_obj.connect_to_signal(
        "PropertiesChanged", _props_changed, dbus_interface=PROPS_IFACE)

    # Check if already connected + resolved
    dev_props = dbus.Interface(dev_obj, PROPS_IFACE)
    try:
        if dev_props.Get(DEVICE_IFACE, "ServicesResolved"):
            resolved[0] = True
    except Exception:
        pass

    if not resolved[0]:
        try:
            device.Connect()
        except dbus.exceptions.DBusException as e:
            print(f"ERROR: Connect() failed — {e}", file=sys.stderr)
            midi_fd.close()
            sys.exit(1)

        GLib.timeout_add(20000, conn_loop.quit)
        conn_loop.run()

    if not resolved[0]:
        print("ERROR: GATT services not resolved within timeout.", file=sys.stderr)
        try:
            device.Disconnect()
        except Exception:
            pass
        midi_fd.close()
        sys.exit(1)

    print("[bridge] Services resolved.")

    # -----------------------------------------------------------------------
    # 3. Find MIDI characteristic and enable notifications
    # -----------------------------------------------------------------------
    char_path = _find_char_path(bus, BLE_MIDI_CHAR_UUID)
    if not char_path:
        print(f"ERROR: MIDI characteristic {BLE_MIDI_CHAR_UUID} not found.",
              file=sys.stderr)
        try:
            device.Disconnect()
        except Exception:
            pass
        midi_fd.close()
        sys.exit(1)

    print(f"[bridge] MIDI char: {char_path}")

    char_obj = bus.get_object(BLUEZ_SERVICE, char_path)
    char_if  = dbus.Interface(char_obj, GATTCHAR_IFACE)

    def _midi_notify(iface, changed, _invalidated):
        if "Value" in changed:
            raw  = bytes(changed["Value"])
            midi = parse_ble_midi(raw)
            if midi:
                midi_fd.write(midi)
                print(f"[midi] {midi.hex(' ')}")

    char_obj.connect_to_signal(
        "PropertiesChanged", _midi_notify, dbus_interface=PROPS_IFACE)

    char_if.StartNotify()
    print("[bridge] Notifications enabled — forwarding MIDI to virmidi.")
    print("[bridge] Running — Ctrl+C to stop.")

    # -----------------------------------------------------------------------
    # 4. Run until disconnected or Ctrl+C
    # -----------------------------------------------------------------------
    run_loop = GLib.MainLoop()

    def _disconnected(iface, changed, _invalidated):
        if iface == DEVICE_IFACE:
            if "Connected" in changed and not bool(changed["Connected"]):
                print("[bridge] Device disconnected.")
                run_loop.quit()

    dev_obj.connect_to_signal(
        "PropertiesChanged", _disconnected, dbus_interface=PROPS_IFACE)

    try:
        run_loop.run()
    except KeyboardInterrupt:
        pass
    finally:
        try:
            char_if.StopNotify()
        except Exception:
            pass
        try:
            device.Disconnect()
        except Exception:
            pass
        midi_fd.close()
        print("\n[bridge] Stopped.")


if __name__ == "__main__":
    main()
