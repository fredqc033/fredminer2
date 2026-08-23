#include <iostream>
#include <string>
#include <vector>

#include "cuda_device.h"

static void usage()
{
    std::cout
        << "FredMiner development build\n\n"
        << "Options:\n"
        << "  --version\n"
        << "  --devices\n"
        << "  --algo <name>\n"
        << "  --pool <host:port>\n"
        << "  --wallet <wallet.worker>\n"
        << "  --pass <password>\n";
}

int main(int argc, char** argv)
{
    if (argc == 1) {
        usage();
        return print_cuda_devices();
    }

    std::string algo;
    std::string pool;
    std::string wallet;
    std::string pass = "x";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--version") {
            std::cout << "FredMiner 0.1.0-dev\n";
            return 0;
        }

        if (arg == "--devices")
            return print_cuda_devices();

        auto take = [&](std::string& out) {
            if (i + 1 >= argc) {
                std::cerr << "Missing value after " << arg << "\n";
                std::exit(2);
            }
            out = argv[++i];
        };

        if (arg == "--algo") take(algo);
        else if (arg == "--pool") take(pool);
        else if (arg == "--wallet") take(wallet);
        else if (arg == "--pass") take(pass);
        else {
            std::cerr << "Unknown option: " << arg << "\n";
            return 2;
        }
    }

    std::cout << "FredMiner 0.1.0-dev\n";
    print_cuda_devices();

    std::cout << "\nConfiguration\n";
    std::cout << "  Algorithm: " << (algo.empty() ? "pearlhash" : algo) << "\n";
    std::cout << "  Pool:      " << (pool.empty() ? "<not set>" : pool) << "\n";
    std::cout << "  Wallet:    " << (wallet.empty() ? "<not set>" : wallet) << "\n";

    std::cerr
        << "\nPearlHash/Stratum core is not implemented in this development build yet.\n"
        << "Do not use this release for production mining.\n";

    return 10;
}
