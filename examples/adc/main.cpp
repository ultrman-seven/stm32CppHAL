#include "wySys.hpp"
#include "wyUsart.hpp"
#include "wyADC.hpp"

int main()
{
    USART::Serial com(1, "a10", "a9", 115200);
    uint16_t adcVal[5] = {5, 5, 5, 5, 5};
    const char *adcPins[] = {"c0", "c1", "c2", "c3", "c4"};
    ADC::dmaWay(1, 5, adcPins, (uint32_t)adcVal);

    int cnt;

    while (1)
    {
        cnt = 5;
        while (cnt--)
            com << "adc[" << cnt << "]val=" << (int)adcVal[cnt] << "\n";
        sys::delayMs(500);
    }
    return 0;
}
