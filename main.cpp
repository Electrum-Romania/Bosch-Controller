#include <sockpp/socket.h>

void sched();

int main()
{
    sockpp::initialize();

	sched();
}
