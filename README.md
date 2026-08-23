# FredMiner

FredMiner is a HiveOS-only custom miner project targeting Pearl (PRL) / PearlHash on NVIDIA RTX 50-series GPUs.

> Status: development scaffold. The HiveOS integration is ready, but the real PearlHash engine and Kryptex Stratum implementation still need to be completed and validated before this can mine PRL.

## Target

- HiveOS / Linux x86_64 only
- NVIDIA RTX 5060 Ti / RTX 5070 Ti first
- CUDA SM120 / Blackwell
- Kryptex pool first
- Multi-GPU
- HiveOS hashrate / accepted / rejected statistics

## Repository layout

```text
FredMiner/
├── CMakeLists.txt
├── src/
├── include/
├── hiveos/
│   ├── h-manifest.conf
│   ├── h-config.sh
│   ├── h-run.sh
│   └── h-stats.sh
├── scripts/
│   └── package.sh
└── docs/
    └── HIVEOS.md
```

## Build on Linux/HiveOS development machine

Required:

- CMake 3.24+
- GCC/G++
- NVIDIA CUDA Toolkit with SM120 support
- NVIDIA driver compatible with that CUDA runtime

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j"$(nproc)"
```

Result:

```text
build/fredminer
```

## Create HiveOS package

After building:

```bash
chmod +x scripts/package.sh
./scripts/package.sh 0.1.0
```

This creates:

```text
dist/fredminer-0.1.0.tar.gz
```

Upload that file to a GitHub Release. The direct Release asset URL becomes the HiveOS **Installation URL**.

## Planned CLI

```bash
./fredminer \
  --algo pearlhash \
  --pool prl-us.kryptex.network:7048 \
  --wallet YOUR_WALLET.WORKER \
  --pass x
```

## Development milestones

1. CUDA / GPU detection
2. Pearl reference implementation
3. Deterministic correctness vectors
4. Kryptex Stratum connection
5. First accepted PRL share
6. SM120 Blackwell kernel
7. Autotuning
8. Kernel fusion / reduced memory traffic
9. Multi-GPU
10. HiveOS production stats and packaging

## Important

Hashrate optimizations must preserve Pearl consensus-valid results. A displayed hashrate is not considered valid performance unless Kryptex accepts the resulting shares.
