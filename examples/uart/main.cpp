#include "wyUsart.hpp"
#include "wySys.hpp"

int main()
{
    USART::Serial com(1, "a10", "a9", 115200);

    while (1)
    {
        com << "hello world\n"
            << "current system time stamp: " << (int)sys::getTimeStamp() << "\n";
        sys::delayMs(500);
    }
    return 0;
}
