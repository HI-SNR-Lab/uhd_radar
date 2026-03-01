# ORCA `.bin` File Format

Raw radar sample files written by `uhd_radar` (`sdr/main.cpp`).

## Byte Layout

- **No header** — pure binary data from the first byte
- **Endianness** — little-endian
- **Sample type** — interleaved `float32` pairs: `(real, imag)` → `complex64` / `fc32`
- **Bytes per complex sample** — 8 bytes (4 bytes real + 4 bytes imag)
- **Samples per record** — `sample_rate × rx_duration` (computed from `*_config.yaml`)
- **Bytes per record** — `samples_per_record × 8`

### Example (test-data session `20251205_164609`)

From `*_config.yaml`:
```
GENERATE.sample_rate = 56e6   # Hz
CHIRP.rx_duration    = 40e-6  # s
CHIRP.num_presums    = 50     # pulses averaged per write
```

- `samples_per_record = 56 000 000 × 40×10⁻⁶ = 2240`
- `bytes_per_record   = 2240 × 8 = 17 920`
- `record_dt          = pulse_rep_int × num_presums = 200×10⁻⁶ × 50 = 0.01 s`
- `total_records      = total_pulses_written / num_presums = 5 207 400 / 50 = 104 148`
- `file_size          = 104 148 × 17 920 = 1 866 332 160 bytes` ✓

## Byte Offset of Record N

```
offset(N) = N × samples_per_record × 8
```

Records are zero-indexed. Record 0 starts at byte 0.

## Python: Reading One Record

```python
import numpy as np

samples_per_record = int(56e6 * 40e-6)   # 2240
bytes_per_record   = samples_per_record * 8

with open("20251205_164609_rx_samps.bin", "rb") as f:
    f.seek(record_idx * bytes_per_record)
    raw = f.read(bytes_per_record)

record = np.frombuffer(raw, dtype="<f4").view(np.complex64)
# record.shape == (2240,)
```

Or read the entire file at once (if memory permits):

```python
data = np.fromfile("20251205_164609_rx_samps.bin", dtype="<f4").view(np.complex64)
data = data.reshape(-1, samples_per_record)   # shape (n_records, samples_per_record)
```

## Timing and Metadata

All metadata is external to the `.bin` file:

| File | Contents |
|------|----------|
| `*_config.yaml` | All radar parameters (`sample_rate`, `rx_duration`, `pulse_rep_int`, `num_presums`, etc.) |
| `*_uhd_stdout.log` | `[START]` wall-clock timestamp, `[ERROR]` chirp indices, `[OPEN/CLOSE FILE]` events, summary counts |
| `*_gpspipe_stdout.log` | GPSD JSON (TPV at ~1 Hz: `lat`, `lon`, `altHAE`, `time`) |
| `*_track.gpx` | GPX trackpoints (fallback GPS source) |

### Timing Reconstruction

```
t[N] = t_start + N × record_dt
```

where `t_start` is the `[START]` wall-clock time from `*_uhd_stdout.log` and
`record_dt = pulse_rep_int × num_presums`.

**Error pulses are skipped** (not written to the file). The `[ERROR]` lines in the
stdout log give the chirp indices of failed pulses. Because presums are counted
across good pulses only, error chirps do not shift record timing — the `N × record_dt`
formula is correct without any error correction.

## Multiple File Segments

When `max_chirps_per_file > 0`, `run.py` splits output into `*.bin.0`, `*.bin.1`, …
and later merges them into a single `*_rx_samps.bin`. The merged file is a simple
concatenation of segments in order.
