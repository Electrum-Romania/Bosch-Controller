#ifndef UTILS_LINE_READER
#define UTILS_LINE_READER

#include <sockpp/tcp_connector.h>

constexpr size_t buffer_size = 1024;

class LineReader {
private:
    sockpp::tcp_connector conn;

    std::vector<uint8_t> buf;

public:
    LineReader(const std::string& host, uint16_t port);

    std::string read_line();

    // conn encapsulation
    explicit operator bool();
    std::string last_error_str();
};

#endif //UTILS_LINE_READER
