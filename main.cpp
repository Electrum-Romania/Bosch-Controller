#include <sockpp/socket.h>

[[noreturn]] void sched();

int main()
{
    sockpp::initialize();

	sched();
}
