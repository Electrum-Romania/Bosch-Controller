#include <utils/command_socket/line_reader.h>

LineReader::LineReader(const std::string& host, uint16_t port)
    : conn({host, port})
{

}

std::string LineReader::read_line()
{
    char b[buffer_size];
    char *p;

    for (;;) {
        ssize_t bytes_read = conn.read(b, buffer_size);

        if (bytes_read == 0)
            return "EOF";
        else if (bytes_read < 0)
            return "E: " + conn.last_error_str();

        p = std::strchr(b, '\n');
        if (p) {
            buf.insert(buf.end(), b, p);

            std::string s(buf.begin(), buf.end());

            buf.clear();

            bytes_read -= p - b;
            std::memmove(b, p, bytes_read);

            return s;
        }

        buf.insert(buf.end(), b, b + bytes_read);
    }
}

LineReader::operator bool()
{
    return (bool) conn;
}

std::string LineReader::last_error_str()
{
    return conn.last_error_str();
}
