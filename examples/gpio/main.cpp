#include "wyGpio.hpp"
#include "wySys.hpp"

int main()
{
    GPIO::GpioPin green("b0");
    green = 1;

    while (1)
    {
        green.flip();
        sys::delayMs(500);
    }

    return 0;
}
