#include "line_reader.h"

line_reader::line_reader(const std::string& host, uint16_t port)
    : conn({host, port})
{

}

std::string line_reader::read_line()
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

line_reader::operator bool()
{
    return (bool) conn;
}

std::string line_reader::last_error_str()
{
    return conn.last_error_str();
}
