#include "stratum.h"

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cctype>
#include <cstdlib>
#include <sstream>

StratumClient::StratumClient(std::string host,
                             std::string port,
                             std::string wallet,
                             std::string password)
    : host_(std::move(host)),
      port_(std::move(port)),
      wallet_(std::move(wallet)),
      password_(std::move(password))
{
}

StratumClient::~StratumClient()
{
    if (sock_ >= 0)
        ::close(sock_);
}

bool StratumClient::connect_tcp()
{
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    addrinfo* result = nullptr;
    const int rc = ::getaddrinfo(host_.c_str(), port_.c_str(), &hints, &result);

    if (rc != 0) {
        std::cerr << "[STRATUM] getaddrinfo: " << gai_strerror(rc) << "\n";
        return false;
    }

    for (addrinfo* p = result; p != nullptr; p = p->ai_next) {
        const int fd = ::socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        if (fd < 0)
            continue;

        if (::connect(fd, p->ai_addr, p->ai_addrlen) == 0) {
            sock_ = fd;
            break;
        }

        ::close(fd);
    }

    ::freeaddrinfo(result);

    if (sock_ < 0) {
        std::cerr << "[STRATUM] unable to connect to "
                  << host_ << ":" << port_ << "\n";
        return false;
    }

    std::cout << "[STRATUM] connected to "
              << host_ << ":" << port_ << "\n";
    return true;
}

std::string StratumClient::json_escape(const std::string& s)
{
    std::ostringstream out;

    for (char c : s) {
        switch (c) {
            case '\\': out << "\\\\"; break;
            case '"':  out << "\\\""; break;
            case '\n': out << "\\n"; break;
            case '\r': out << "\\r"; break;
            case '\t': out << "\\t"; break;
            default:   out << c; break;
        }
    }

    return out.str();
}

bool StratumClient::send_line(const std::string& line)
{
    std::string data = line;
    data.push_back('\n');

    const char* p = data.data();
    std::size_t left = data.size();

    while (left > 0) {
        const ssize_t n = ::send(sock_, p, left, MSG_NOSIGNAL);

        if (n <= 0) {
            std::cerr << "[STRATUM] send failed: "
                      << std::strerror(errno) << "\n";
            return false;
        }

        p += n;
        left -= static_cast<std::size_t>(n);
    }

    return true;
}

bool StratumClient::authorize()
{
    const std::uint64_t id = next_id_++;

    // V1-compatible positional authorize.
    // Some Pearl pools accept wallet.worker as the first parameter.
    std::ostringstream req;
    req << "{\"id\":" << id
        << ",\"method\":\"mining.authorize\",\"params\":[\""
        << json_escape(wallet_) << "\",\""
        << json_escape(password_) << "\"]}";

    if (!send_line(req.str()))
        return false;

    std::cout << "[STRATUM] authorize sent for "
              << wallet_ << "\n";
    return true;
}

static std::size_t find_value_start(const std::string& json,
                                    const std::string& key)
{
    const std::string needle = "\"" + key + "\"";
    std::size_t pos = json.find(needle);

    if (pos == std::string::npos)
        return pos;

    pos = json.find(':', pos + needle.size());
    if (pos == std::string::npos)
        return pos;

    ++pos;
    while (pos < json.size() &&
           std::isspace(static_cast<unsigned char>(json[pos])))
        ++pos;

    return pos;
}

bool StratumClient::json_string_field(const std::string& json,
                                      const std::string& key,
                                      std::string& value)
{
    std::size_t pos = find_value_start(json, key);

    if (pos == std::string::npos || pos >= json.size() || json[pos] != '"')
        return false;

    ++pos;
    std::ostringstream out;
    bool escaped = false;

    for (; pos < json.size(); ++pos) {
        const char c = json[pos];

        if (escaped) {
            out << c;
            escaped = false;
            continue;
        }

        if (c == '\\') {
            escaped = true;
            continue;
        }

        if (c == '"') {
            value = out.str();
            return true;
        }

        out << c;
    }

    return false;
}

bool StratumClient::json_u64_field(const std::string& json,
                                   const std::string& key,
                                   std::uint64_t& value)
{
    const std::size_t pos = find_value_start(json, key);

    if (pos == std::string::npos)
        return false;

    char* end = nullptr;
    const unsigned long long v =
        std::strtoull(json.c_str() + pos, &end, 10);

    if (end == json.c_str() + pos)
        return false;

    value = static_cast<std::uint64_t>(v);
    return true;
}

bool StratumClient::json_double_field(const std::string& json,
                                      const std::string& key,
                                      double& value)
{
    const std::size_t pos = find_value_start(json, key);

    if (pos == std::string::npos)
        return false;

    char* end = nullptr;
    const double v = std::strtod(json.c_str() + pos, &end);

    if (end == json.c_str() + pos)
        return false;

    value = v;
    return true;
}

bool StratumClient::parse_object_notify(const std::string& line,
                                        PearlJob& job)
{
    if (line.find("\"method\":\"mining.notify\"") == std::string::npos &&
        line.find("\"method\": \"mining.notify\"") == std::string::npos)
        return false;

    if (!json_string_field(line, "job_id", job.job_id))
        return false;

    if (!json_string_field(line, "header", job.header_hex))
        return false;

    if (!json_string_field(line, "target", job.target_hex))
        return false;

    json_u64_field(line, "height", job.height);
    json_double_field(line, "difficulty", job.difficulty);

    return true;
}

bool StratumClient::handle_line(const std::string& line)
{
    std::cout << "[RX] " << line << "\n";

    PearlJob job;

    if (parse_object_notify(line, job)) {
        std::cout << "[JOB] id=" << job.job_id
                  << " height=" << job.height
                  << " diff=" << job.difficulty
                  << "\n";

        std::cout << "[JOB] header bytes="
                  << (job.header_hex.size() / 2)
                  << " target bytes="
                  << (job.target_hex.size() / 2)
                  << "\n";

        if (job.header_hex.size() != 152)
            std::cout << "[WARN] V1 header should be 152 hex chars\n";

        if (job.target_hex.size() != 64)
            std::cout << "[WARN] V1 target should be 64 hex chars\n";

        return true;
    }

    if (line.find("mining.notify") != std::string::npos) {
        std::cout
            << "[JOB] mining.notify received in an unparsed dialect.\n"
            << "[JOB] Keep this raw line; we will add the exact Kryptex parser.\n";
    }

    return true;
}

int StratumClient::run()
{
    std::string pending;
    char buffer[8192];

    for (;;) {
        const ssize_t n = ::recv(sock_, buffer, sizeof(buffer), 0);

        if (n == 0) {
            std::cerr << "[STRATUM] pool closed connection\n";
            return 3;
        }

        if (n < 0) {
            if (errno == EINTR)
                continue;

            std::cerr << "[STRATUM] recv failed: "
                      << std::strerror(errno) << "\n";
            return 4;
        }

        pending.append(buffer, static_cast<std::size_t>(n));

        for (;;) {
            const std::size_t nl = pending.find('\n');

            if (nl == std::string::npos)
                break;

            std::string line = pending.substr(0, nl);
            pending.erase(0, nl + 1);

            if (!line.empty() && line.back() == '\r')
                line.pop_back();

            if (!line.empty())
                handle_line(line);
        }
    }
}
