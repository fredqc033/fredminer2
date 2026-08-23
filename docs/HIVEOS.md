# HiveOS setup

FredMiner is packaged as a HiveOS Custom Miner.

HiveOS' official custom-miner format uses:

- `h-manifest.conf`
- `h-config.sh`
- `h-run.sh`
- `h-stats.sh`
- the miner binary

## Flight Sheet

Use:

- Miner: Custom
- Miner name: `fredminer`
- Hash algorithm: `pearlhash`
- Wallet and worker template: your Kryptex/Pearl wallet template
- Pool URL: `prl-us.kryptex.network:7048`
- Pass: `x`
- Extra config arguments: leave empty initially

## Installation URL

After publishing `fredminer-X.Y.Z.tar.gz` as a GitHub Release asset:

```text
https://github.com/YOUR_USERNAME/YOUR_REPO/releases/download/vX.Y.Z/fredminer-X.Y.Z.tar.gz
```

The exact URL depends on the GitHub username, repository name, tag and release asset name.

## Current status

The wrapper is ready for HiveOS, but the current development binary does not yet implement PearlHash or Stratum. Do not use it for production mining until a release is explicitly marked mining-ready.
