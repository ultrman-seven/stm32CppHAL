#include "chipf10x.hpp"
// #include "wyGpio.hpp"
namespace SPI
{
    class SPI_Object // mosi,miso,sclk
    {
    private:
        SPI_TypeDef *spi;

    public:
        SPI_Object(uint8_t num, const char *mosi, const char *miso, const char *sclk);
        // void sendOneByte(uint8_t dat);
        // uint8_t read(void);
        uint32_t writeRead(uint8_t dat = 0xff);
        SPI_Object &operator>>(uint8_t &rd);
        SPI_Object &operator<<(uint8_t wt);
    };
} // namespace SPI

#include "wyGpio.hpp"

using namespace SPI;

SPI_Object::SPI_Object(uint8_t num, const char *mosi, const char *miso, const char *sclk)
{
    if (mosi != nullptr)
    {
        GPIO::modeConfig(mosi, GPIO::Mode_AF_PP);
    }
    if (sclk != nullptr)
    {
        GPIO::modeConfig(sclk, GPIO::Mode_AF_PP);
    }
    if (miso != nullptr)
    {
        GPIO::modeConfig(sclk, GPIO::Mode_IN_FLOATING);
    }
}
