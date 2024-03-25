#ifndef EE2DDE29_77F8_423A_8314_4B843FECD94D
#define EE2DDE29_77F8_423A_8314_4B843FECD94D
#include "chipf10x.hpp"

namespace USART
{
    class Serial
    {
    private:
        USART_TypeDef *uart;

    public:
        Serial(uint8_t, const char *rx = nullptr, const char *tx = nullptr, uint32_t = 115200);

        void sendByte(uint8_t dat);
        void sendByte(uint8_t *dat, uint8_t len);

        Serial &operator<<(uint8_t dat);
        Serial &operator<<(const char dat);
        Serial &operator<<(int32_t num);
        Serial &operator<<(float num);
        Serial &operator<<(const char *s);
        Serial &operator<<(char *s);

        Serial &operator>>(uint8_t &dat);

        uint8_t receiveByte(void);
    };

} // namespace USART

#endif /* EE2DDE29_77F8_423A_8314_4B843FECD94D */
