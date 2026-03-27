#!/usr/bin/env python3
"""
ble_midi_bridge.py

BLE MIDI → ALSA virtual MIDI bridge.

Connects to a BLE MIDI peripheral (e.g. CME WIDI Core) via BlueZ D-Bus,
subscribes to MIDI GATT notifications, strips the BLE MIDI header/timestamp
bytes, and writes raw MIDI to a snd-virmidi device so the rest of the
ALSA/control chain sees it as a normal sequencer port.

Dependencies (all in Patchbox apt repos — no pip needed):
    sudo apt install python3-dbus python3-gi
    sudo modprobe snd-virmidi midi_devs=1

Usage:
    python3 ble_midi_bridge.py [BLE_MAC_ADDRESS]

    Default MAC: F0:8C:F3:8B:63:04  (WIDI Core discovered on this system)

BLE MIDI packet format (MIDI over BLE GATT spec):
    Byte 0  : Header    — timestamp MSB  (bit7=1, bit6=0)
    Byte 1  : Timestamp — timestamp LSB  (bit7=1)
    Bytes 2+: MIDI status + data bytes, possibly interleaved with
              more timestamp bytes before each new MIDI status byte.

    Parsing rule used here: a byte with bit7=1 that is immediately followed
    by another byte with bit7=1 is a timestamp → skip it.
    Everything else is a real MIDI byte.
"""

import sys
import os
import time

DEFAULT_MAC = "F0:8C:F3:8B:63:04"

# BLE MIDI GATT UUIDs (standardised, device-independent)
BLE_MIDI_SERVICE_UUID = "03b80e5a-ede8-4b33-a751-6ce34ec4c700"
BLE_MIDI_CHAR_UUID    = "7772e5db-3868-4112-a1a9-f2669d106bf3"


# ---------------------------------------------------------------------------
# BLE MIDI packet parser
# ---------------------------------------------------------------------------

def parse_ble_midi(data: bytes) -> bytes:
    """
    Strip BLE MIDI framing and return raw MIDI bytes.

    A byte with bit7=1 is a timestamp when the NEXT byte also has bit7=1
    (i.e. it precedes a MIDI status byte).  In all other positions, a byte
    with bit7=1 is a MIDI status byte.
    """
    if len(data) < 3:
        return b''

    out = []
    i = 1  # skip header byte (index 0)

    while i < len(data):
        b = data[i]
        if b & 0x80:
            # Peek at next byte
            if i + 1 < len(data) and (data[i + 1] & 0x80):
                # This byte is a timestamp — skip it; next is MIDI status
                i += 1
                continue
            else:
                # This IS the MIDI status byte
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
    os.system("sudo modprobe snd-virmidi midi_devs=1 2>/dev/null")
    time.sleep(0.5)

    for path in VIRMIDI_CANDIDATES:
        if os.path.exists(path):
            print(f"[bridge] Virtual MIDI device: {path}")
            return open(path, 'wb', buffering=0)

    print("ERROR: no snd-virmidi device found after modprobe.", file=sys.stderr)
    print("       Run:  sudo modprobe snd-virmidi midi_devs=1", file=sys.stderr)
    sys.exit(1)


# ---------------------------------------------------------------------------
# BlueZ D-Bus helpers
# ---------------------------------------------------------------------------

def mac_to_dbus_path(mac: str) -> str:
    return "/org/bluez/hci0/dev_" + mac.replace(":", "_")


def find_midi_characteristic(bus, dev_path: str) -> str:
    """
    Walk BlueZ managed objects to find the BLE MIDI characteristic path
    under dev_path.  Returns the D-Bus object path or raises RuntimeError.
    """
    import dbus

    obj_mgr = dbus.Interface(
        bus.get_object("org.bluez", "/"),
        "org.freedesktop.DBus.ObjectManager"
    )
    objects = obj_mgr.GetManagedObjects()

    for path, ifaces in objects.items():
        if not str(path).startswith(dev_path):
            continue
        if "org.bluez.GATTCharacteristic1" not in ifaces:
            continue
        uuid = str(ifaces["org.bluez.GATTCharacteristic1"].get("UUID", "")).lower()
        if uuid == BLE_MIDI_CHAR_UUID:
            return str(path)

    raise RuntimeError(
        "BLE MIDI characteristic not found under " + dev_path +
        "\nIs the device connected and GATT services resolved?"
    )


def ensure_connected(bus, dev_path: str):
    """Connect to the BLE device if not already connected."""
    import dbus

    dev_obj   = bus.get_object("org.bluez", dev_path)
    dev_iface = dbus.Interface(dev_obj, "org.bluez.Device1")
    props     = dbus.Interface(dev_obj, "org.freedesktop.DBus.Properties")

    connected = bool(props.Get("org.bluez.Device1", "Connected"))
    if connected:
        print("[bridge] Already connected.")
        return

    print("[bridge] Connecting …")
    dev_iface.Connect()

    # Wait up to 10 s for GATT services to resolve
    for _ in range(20):
        time.sleep(0.5)
        try:
            connected  = bool(props.Get("org.bluez.Device1", "Connected"))
            resolved   = bool(props.Get("org.bluez.Device1", "ServicesResolved"))
        except Exception:
            connected  = False
            resolved   = False
        if connected and resolved:
            print("[bridge] Connected and GATT services resolved.")
            return

    raise RuntimeError("Timed out waiting for BLE connection / GATT resolve.")


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    mac = sys.argv[1] if len(sys.argv) > 1 else DEFAULT_MAC
    print(f"[bridge] Target BLE device: {mac}")

    midi_fd = open_virmidi()

    try:
        import dbus
        import dbus.mainloop.glib
        from gi.repository import GLib
    except ImportError as e:
        print(f"ERROR: missing dependency — {e}", file=sys.stderr)
        print("       sudo apt install python3-dbus python3-gi", file=sys.stderr)
        sys.exit(1)

    dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
    bus      = dbus.SystemBus()
    dev_path = mac_to_dbus_path(mac)

    ensure_connected(bus, dev_path)

    char_path = find_midi_characteristic(bus, dev_path)
    print(f"[bridge] MIDI characteristic: {char_path}")

    # Notification callback
    def on_properties_changed(interface, changed, invalidated, path=None):
        if interface != "org.bluez.GATTCharacteristic1":
            return
        if "Value" not in changed:
            return
        raw  = bytes(changed["Value"])
        midi = parse_ble_midi(raw)
        if midi:
            midi_fd.write(midi)
            print(f"[midi] {midi.hex(' ')}")

    bus.add_signal_receiver(
        on_properties_changed,
        dbus_interface="org.freedesktop.DBus.Properties",
        signal_name="PropertiesChanged",
        path=char_path,
        path_keyword="path",
    )

    char_obj   = bus.get_object("org.bluez", char_path)
    char_iface = dbus.Interface(char_obj, "org.bluez.GATTCharacteristic1")
    char_iface.StartNotify()

    print("[bridge] Running — Ctrl+C to stop.")
    loop = GLib.MainLoop()
    try:
        loop.run()
    except KeyboardInterrupt:
        pass
    finally:
        try:
            char_iface.StopNotify()
        except Exception:
            pass
        midi_fd.close()
        print("\n[bridge] Stopped.")


if __name__ == "__main__":
    main()
