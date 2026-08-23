#include <cuda_runtime.h>
#include <iostream>
#include "cuda_device.h"

int print_cuda_devices()
{
    int count = 0;
    cudaError_t e = cudaGetDeviceCount(&count);

    if (e != cudaSuccess) {
        std::cerr << "[CUDA] cudaGetDeviceCount: "
                  << cudaGetErrorString(e) << "\n";
        return 1;
    }

    std::cout << "[CUDA] GPUs detected: " << count << "\n";

    for (int i = 0; i < count; ++i) {
        cudaDeviceProp p{};
        e = cudaGetDeviceProperties(&p, i);

        if (e != cudaSuccess) {
            std::cerr << "[CUDA] cudaGetDeviceProperties(" << i << "): "
                      << cudaGetErrorString(e) << "\n";
            return 2;
        }

        std::cout << "[CUDA] GPU " << i << ": " << p.name
                  << " | CC " << p.major << "." << p.minor
                  << " | SM " << p.multiProcessorCount
                  << " | VRAM "
                  << (p.totalGlobalMem / (1024ull * 1024ull))
                  << " MB\n";
    }

    return 0;
}
