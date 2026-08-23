# Next milestone: Pearl correctness engine

Pearl's public miner code separates:

1. deterministic noise generation
2. noise A
3. noise B
4. noisy GEMM
5. denoising/reduction
6. PoW extraction / BLAKE3
7. plain-proof construction

The public settings currently show a base noise rank of 128 and GEMM tiles
including 128 x 256 x 128.

FredMiner's next step is **not** optimization yet. First we need deterministic
test vectors so the same input produces exactly the same proof/hash as Pearl's
reference implementation.

Only after correctness is established should the SM120 path be optimized using:

- job-constant data reuse
- persistent GPU buffers
- fused stages where mathematically equivalent
- reduced global-memory traffic
- Blackwell-specific tile/stage autotuning
- multi-stream / multi-GPU scheduling

Every optimization must preserve consensus-valid output.
