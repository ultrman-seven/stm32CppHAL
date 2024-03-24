#include "chipf10x.h"
// #include "wyGpio.hpp"
namespace SPI
{
    class SPI_Object // mosi,miso,sclk
    {
    private:
        SPI_TypeDef *spi;
        // GPIO::GpioPin *sclk = nullptr, *mosi = nullptr, *miso = nullptr;

    protected:
        void init(const char *mosi, const char *miso, const char *sclk);
        void init(uint8_t num, const char *mosi, const char *miso, const char *sclk);


    public:
        SPI_Object(/* args */) = default;
        SPI_Object(const char *mosi, const char *miso, const char *sclk)
        {
            init(mosi, miso, sclk);
        }

        SPI_Object(uint8_t num, const char *mosi, const char *miso, const char *sclk)
        {
            init(num, mosi, miso, sclk);
        }
        // void sendOneByte(uint8_t dat);
        // uint8_t read(void);
        uint32_t writeRead(uint8_t dat = 0xff);
        SPI_Object &operator>>(uint8_t &rd);
        SPI_Object &operator<<(uint8_t wt);
    };
} // namespace SPI

using namespace SPI;
