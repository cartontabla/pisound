#!/usr/bin/env bash
set -euo pipefail

PROJECT="${1:?PROJECT (e.g. amp)}"
TTL="${2:?path/to/plugin.ttl}"
OUT_DIR="${3:-}"

command -v python3 >/dev/null 2>&1 || { echo "[gen_lv2] ERROR: falta python3" >&2; exit 1; }
[[ -f "$TTL" ]] || { echo "[gen_lv2] ERROR: no existe $TTL" >&2; exit 1; }

# Normaliza a rutas absolutas (independiente del cwd)
TTL="$(python3 -c 'import os,sys; print(os.path.abspath(sys.argv[1]))' "$TTL")"
OUT_DIR="${OUT_DIR:-$(dirname "$TTL")}"
OUT_DIR="$(python3 -c 'import os,sys; print(os.path.abspath(sys.argv[1]))' "$OUT_DIR")"
OUT="${OUT_DIR}/${PROJECT}_lv2.c"

echo "[gen_lv2] PROJECT=$PROJECT" >&2
echo "[gen_lv2] TTL=$TTL" >&2
echo "[gen_lv2] OUT=$OUT" >&2

mkdir -p "$OUT_DIR"

python3 - "$PROJECT" "$TTL" "$OUT" <<'PY'
import re, sys
project, ttl_path, out_path = sys.argv[1], sys.argv[2], sys.argv[3]

plugin_uri = None
ports = []
subj_candidate = None
in_ports = False
cur = None

uri_line = re.compile(r'^\s*<([^>]+)>\s*$')
plugin_decl = re.compile(r'\ba\s+lv2:Plugin\b')
inport_decl  = re.compile(r'\ba\s+lv2:InputPort\b')
outport_decl = re.compile(r'\ba\s+lv2:OutputPort\b')
idx_re = re.compile(r'\blv2:index\s+(\d+)')
sym_re = re.compile(r'\blv2:symbol\s+"([^"]+)"')

with open(ttl_path, 'r', encoding='utf-8', errors='replace') as f:
    for line in f:
        m = uri_line.match(line)
        if m:
            subj_candidate = m.group(1)
            continue
        if subj_candidate and plugin_decl.search(line):
            plugin_uri = subj_candidate
            subj_candidate = None

        if re.match(r'^\s*lv2:port\s*$', line):
            in_ports = True
            continue
        if not in_ports:
            continue

        if inport_decl.search(line) or outport_decl.search(line):
            cur = {
                "dir": "in" if inport_decl.search(line) else "out",
                "kind": "audio" if "lv2:AudioPort" in line else ("control" if "lv2:ControlPort" in line else ""),
                "index": None,
                "symbol": None
            }
            continue

        if cur is None:
            continue

        m = idx_re.search(line)
        if m: cur["index"] = int(m.group(1))
        m = sym_re.search(line)
        if m: cur["symbol"] = m.group(1)

        if re.match(r'^\s*\]', line):
            if cur["index"] is not None and cur["symbol"] and cur["dir"] and cur["kind"]:
                ports.append(cur)
            cur = None

        if re.search(r'\]\s*\.\s*$', line):
            break

if not plugin_uri:
    raise SystemExit(f"[gen_lv2] ERROR: no pude extraer PLUGIN_URI de {ttl_path}")
if not ports:
    raise SystemExit(f"[gen_lv2] ERROR: no pude parsear puertos de {ttl_path}")

ports.sort(key=lambda p: p["index"])

def sanitize(s: str) -> str:
    s = re.sub(r'[^A-Za-z0-9_]', '_', s)
    if re.match(r'^\d', s): s = '_' + s
    return s

def upper(s: str) -> str:
    return s.upper()

audio_in, audio_out = [], []
gain_field = None

for p in ports:
    sym = sanitize(p["symbol"])
    if p["kind"] == "audio" and p["dir"] == "in":  audio_in.append(sym)
    if p["kind"] == "audio" and p["dir"] == "out": audio_out.append(sym)
    if p["kind"] == "control" and p["dir"] == "in" and p["symbol"] == "gain":
        gain_field = sym

enum_names = {p["index"]: f"PORT_{upper(sanitize(p['symbol']))}" for p in ports}
field_types = {sanitize(p["symbol"]): ("const float*" if p["dir"]=="in" else "float*") for p in ports}

with open(out_path, "w", encoding="utf-8") as o:
    o.write(f"""\
#include <lv2/core/lv2.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PLUGIN_URI "{plugin_uri}"

typedef enum {{
""")
    for p in ports:
        o.write(f"  {enum_names[p['index']]} = {p['index']},\n")
    o.write("""\
} PortIndex;

typedef struct {
""")
    for p in ports:
        field = sanitize(p["symbol"])
        o.write(f"  {field_types[field]} {field};\n")
    o.write("""\
} Plugin;

static LV2_Handle instantiate(const LV2_Descriptor* d, double r,
                              const char* b, const LV2_Feature* const* f)
{
  (void)d; (void)r; (void)b; (void)f;
  return (LV2_Handle)calloc(1, sizeof(Plugin));
}

static void connect_port(LV2_Handle inst, uint32_t port, void* data)
{
  Plugin* self = (Plugin*)inst;
  switch ((PortIndex)port) {
""")
    for p in ports:
        field = sanitize(p["symbol"])
        o.write(f"    case {enum_names[p['index']]}: self->{field} = ({field_types[field]})data; break;\n")
    o.write("""\
    default: break;
  }
}

static void activate(LV2_Handle inst) { (void)inst; }
static inline float db_to_lin(float db) { return powf(10.0f, db / 20.0f); }

static void run(LV2_Handle inst, uint32_t n_samples)
{
  Plugin* self = (Plugin*)inst;
  if (!self || n_samples == 0) return;
""")
    for out in audio_out:
        o.write(f"  if (!self->{out}) return;\n")

    o.write("\n  float g = 1.0f;\n")
    if gain_field:
        o.write(f"""\
  if (self->{gain_field}) {{
    const float db = *self->{gain_field};
    g = (db <= -60.0f) ? 0.0f : db_to_lin(db);
  }}
""")

    if audio_out:
        if not audio_in:
            for out in audio_out:
                o.write(f"  memset(self->{out}, 0, (size_t)n_samples * sizeof(float));\n")
        else:
            for j, out in enumerate(audio_out):
                src = audio_in[min(j, len(audio_in)-1)]
                o.write(f"""\
  if (!self->{src}) {{
    memset(self->{out}, 0, (size_t)n_samples * sizeof(float));
  }} else {{
    for (uint32_t i = 0; i < n_samples; ++i) self->{out}[i] = self->{src}[i] * g;
  }}

""")

    o.write("""\
}

static void deactivate(LV2_Handle inst) { (void)inst; }
static void cleanup(LV2_Handle inst) { free(inst); }
static const void* extension_data(const char* uri) { (void)uri; return NULL; }

static const LV2_Descriptor descriptor = {
  PLUGIN_URI, instantiate, connect_port, activate,
  run, deactivate, cleanup, extension_data
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
  return (index == 0) ? &descriptor : NULL;
}
""")

print("[gen_lv2] OK:", out_path, file=sys.stderr)
PY

ls -l "$OUT" >&2
