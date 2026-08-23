#pragma once

#include <cstdint>
#include <string>

struct PearlJob {
    std::string job_id;
    std::string header_hex;
    std::string target_hex;
    std::uint64_t height = 0;
    double difficulty = 0.0;
};

class StratumClient {
public:
    StratumClient(std::string host,
                  std::string port,
                  std::string wallet,
                  std::string password);

    ~StratumClient();

    bool connect_tcp();
    bool authorize();
    int run();

private:
    std::string host_;
    std::string port_;
    std::string wallet_;
    std::string password_;

    int sock_ = -1;
    std::uint64_t next_id_ = 1;

    bool send_line(const std::string& line);
    bool handle_line(const std::string& line);
    bool parse_object_notify(const std::string& line, PearlJob& job);

    static std::string json_escape(const std::string& s);
    static bool json_string_field(const std::string& json,
                                  const std::string& key,
                                  std::string& value);
    static bool json_u64_field(const std::string& json,
                               const std::string& key,
                               std::uint64_t& value);
    static bool json_double_field(const std::string& json,
                                  const std::string& key,
                                  double& value);
};
