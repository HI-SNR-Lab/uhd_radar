# ORCA `.bin` File Format

Raw radar sample files written by `uhd_radar`
([`sdr/main.cpp`](https://github.com/HI-SNR-Lab/uhd_radar/blob/28be9fa86cb4d4b6abf8ffa955b871af4479b3a6/sdr/main.cpp)).

## Byte Layout

- **No header** — pure binary data from the first byte
  ([`main.cpp:582-583`](https://github.com/HI-SNR-Lab/uhd_radar/blob/28be9fa86cb4d4b6abf8ffa955b871af4479b3a6/sdr/main.cpp#L582-L583):
  `outfile.write((const char*)&sample_sum.front(), num_rx_samps * sizeof(complex<float>))`)
- **Endianness** — little-endian (host byte order; USRP host code always runs on x86/ARM little-endian)
- **Sample type** — interleaved `float32` pairs: `(real, imag)` → `complex64` / `fc32`
  (only `fc32` is supported: [`main.cpp:506-507`](https://github.com/HI-SNR-Lab/uhd_radar/blob/28be9fa86cb4d4b6abf8ffa955b871af4479b3a6/sdr/main.cpp#L506-L507);
  format described in [`processing.py:122-124`](https://github.com/HI-SNR-Lab/uhd_radar/blob/28be9fa86cb4d4b6abf8ffa955b871af4479b3a6/postprocessing/processing.py#L122-L124))
- **Bytes per complex sample** — 8 bytes (4 bytes real + 4 bytes imag)
- **Samples per record** — `sample_rate × rx_duration` (computed from `*_config.yaml`)
  ([`main.cpp:184`](https://github.com/HI-SNR-Lab/uhd_radar/blob/28be9fa86cb4d4b6abf8ffa955b871af4479b3a6/sdr/main.cpp#L184):
  `num_rx_samps = rx_rate * rx_duration`)
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

The existing read helper in `postprocessing/processing.py` is
[`extractSig()`](https://github.com/HI-SNR-Lab/uhd_radar/blob/28be9fa86cb4d4b6abf8ffa955b871af4479b3a6/postprocessing/processing.py#L128-L130),
which reads `float32` values and reconstructs complex pairs as
`sig_floats[::2] + 1j * sig_floats[1::2]`.  The equivalent using
`numpy.view` (avoids a copy):

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

### Log format

The stdout log entries cited below are emitted by `main.cpp` and are explicitly
noted in comments as used by post-processing code (format changes require care):

| Log marker | Source line | Example |
|-----------|-------------|---------|
| `[START]` | [`main.cpp:530`](https://github.com/HI-SNR-Lab/uhd_radar/blob/28be9fa86cb4d4b6abf8ffa955b871af4479b3a6/sdr/main.cpp#L530) | `[1764952121.787] [START] Beginning main loop` |
| `[ERROR]` | [`main.cpp:543`](https://github.com/HI-SNR-Lab/uhd_radar/blob/28be9fa86cb4d4b6abf8ffa955b871af4479b3a6/sdr/main.cpp#L543) | `[...] [ERROR] (Chirp 140907) Receiver error: ERROR_CODE_LATE_COMMAND` |
| `[OPEN FILE]` / `[CLOSE FILE]` | [`main.cpp:496`](https://github.com/HI-SNR-Lab/uhd_radar/blob/28be9fa86cb4d4b6abf8ffa955b871af4479b3a6/sdr/main.cpp#L496), [`main.cpp:617-625`](https://github.com/HI-SNR-Lab/uhd_radar/blob/28be9fa86cb4d4b6abf8ffa955b871af4479b3a6/sdr/main.cpp#L617-L625) | `[...] [OPEN FILE] ../../data/rx_samps.bin.0` |
| `[RX] Error count` / `Total pulses written` / `Total pulses attempted` | [`main.cpp:642-644`](https://github.com/HI-SNR-Lab/uhd_radar/blob/28be9fa86cb4d4b6abf8ffa955b871af4479b3a6/sdr/main.cpp#L642-L644) | `[RX] Total pulses written: 5207400` |

### Timing Reconstruction

```
t[N] ≈ t_start + N × record_dt
```

where `t_start` is the `[START]` wall-clock time from `*_uhd_stdout.log` and
`record_dt = pulse_rep_int × num_presums`.

**Error pulses are skipped** (not written to the file). The `[ERROR]` lines in the
stdout log give the chirp indices of failed pulses. Because each record accumulates
exactly `num_presums` **good** chirps
([`main.cpp:578`](https://github.com/HI-SNR-Lab/uhd_radar/blob/28be9fa86cb4d4b6abf8ffa955b871af4479b3a6/sdr/main.cpp#L578):
`(pulses_received - error_count) % num_presums == 0`),
an error pulse extends the collection window by one `pulse_rep_int`. The exact
timestamp of record N is:

```
t[N] = t_start + (N × num_presums + errors_before_N) × pulse_rep_int
```

where `errors_before_N` is the count of error chirps that fell within records 0…N−1.
The simple `N × record_dt` formula is therefore an approximation that underestimates
the actual time by `errors_before_N × pulse_rep_int`. The maximum timing error at the
end of a session equals `total_errors × pulse_rep_int` (e.g. 617 errors × 200 µs =
0.12 s for the test-data session — small relative to the 1 Hz GPS sample rate, but
non-zero).

For higher-accuracy timing, count errors from the log up to each record boundary and
apply the exact formula above.

## Multiple File Segments

When `max_chirps_per_file > 0`, `run.py` splits output into `*.bin.0`, `*.bin.1`, …
and later merges them into a single `*_rx_samps.bin`. The merged file is a simple
concatenation of segments in order.
