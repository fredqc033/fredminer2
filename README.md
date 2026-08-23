# FredMiner v0.2-dev

HiveOS-only PearlHash miner project for NVIDIA RTX 50-series GPUs.

## Current milestone

This build adds:

- Linux/HiveOS CUDA device detection
- TCP Stratum client
- `mining.authorize`
- newline-delimited JSON-RPC receive loop
- Pearl V1 object-style `mining.notify` parsing
- raw logging for alternate/positional Pearl Stratum dialects
- HiveOS Custom Miner scripts

It **does not generate Pearl proofs yet**, so it cannot submit valid shares yet.

The next milestone is the Pearl reference/correctness engine, followed by the
SM120 Blackwell implementation.

## Kryptex target

North America:

```text
prl-us.kryptex.network:7048
```

Global:

```text
prl.kryptex.network:7048
```

Kryptex currently lists TCP on port 7048 and SSL on 8048.

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j"$(nproc)"
```

## Connection test

```bash
./build/fredminer \
  --pool prl-us.kryptex.network:7048 \
  --wallet YOUR_PRL_WALLET.WORKER \
  --pass x \
  --devices
```

Then run without `--devices`:

```bash
./build/fredminer \
  --pool prl-us.kryptex.network:7048 \
  --wallet YOUR_PRL_WALLET.WORKER \
  --pass x
```

Expected result for this development milestone:

```text
[STRATUM] connected
[STRATUM] authorize sent
[RX] ...
[JOB] ...
```

If Kryptex uses a positional Pearl dialect, FredMiner will print the raw
`mining.notify` line so we can adapt the parser from a real session.

## Important

No reported TH/s is implemented yet. FredMiner will only report hashrate once
the Pearl computation is real and verified.
