#include "wyGpio.hpp"

GPIO::GpioPin green("b0");
void greenFlip(void) { green.flip(); }

int main()
{
    green = 1;
    // GPIO::extiConfig("a0", greenFlip);
    GPIO::extiConfig("a0", greenFlip,GPIO::Mode_IN_FLOATING);

    while (1)
        ;
    return 0;
}
