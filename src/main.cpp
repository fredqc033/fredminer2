#include <iostream>
#include <string>

#include "cuda_device.h"
#include "stratum.h"

static void usage()
{
    std::cout <<
R"(FredMiner v0.2-dev

Usage:
  fredminer --pool HOST:PORT --wallet WALLET.WORKER [--pass x]
  fredminer --devices
  fredminer --version

This development milestone connects to Pearl Stratum and receives jobs.
It does not generate or submit Pearl proofs yet.
)";
}

static bool split_pool(const std::string& input,
                       std::string& host,
                       std::string& port)
{
    std::string s = input;

    const std::string tcp = "stratum+tcp://";
    if (s.rfind(tcp, 0) == 0)
        s.erase(0, tcp.size());

    const std::size_t colon = s.rfind(':');

    if (colon == std::string::npos || colon == 0 || colon + 1 >= s.size())
        return false;

    host = s.substr(0, colon);
    port = s.substr(colon + 1);
    return true;
}

int main(int argc, char** argv)
{
    std::string pool;
    std::string wallet;
    std::string pass = "x";
    bool devices_only = false;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];

        auto take = [&](std::string& out) {
            if (i + 1 >= argc) {
                std::cerr << "Missing value after " << arg << "\n";
                std::exit(2);
            }
            out = argv[++i];
        };

        if (arg == "--pool") take(pool);
        else if (arg == "--wallet") take(wallet);
        else if (arg == "--pass") take(pass);
        else if (arg == "--devices") devices_only = true;
        else if (arg == "--version") {
            std::cout << "FredMiner 0.2.0-dev\n";
            return 0;
        }
        else if (arg == "--algo") {
            std::string ignored;
            take(ignored);
        }
        else {
            std::cerr << "Unknown option: " << arg << "\n";
            usage();
            return 2;
        }
    }

    std::cout << "FredMiner 0.2.0-dev\n";
    print_cuda_devices();

    if (devices_only)
        return 0;

    if (pool.empty() || wallet.empty()) {
        usage();
        return 2;
    }

    std::string host;
    std::string port;

    if (!split_pool(pool, host, port)) {
        std::cerr << "Invalid pool. Expected HOST:PORT\n";
        return 2;
    }

    StratumClient client(host, port, wallet, pass);

    if (!client.connect_tcp())
        return 3;

    if (!client.authorize())
        return 4;

    return client.run();
}
