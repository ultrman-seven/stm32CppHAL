#ifndef CD0E5873_EDE4_4337_A345_A2D0DE941A80
#define CD0E5873_EDE4_4337_A345_A2D0DE941A80
#include "stm32f10x.h"
namespace GPIO
{
    typedef enum
    {                            //      xxxx I/O|mode
        Mode_AIN = 0x00,    // = 0b 0000 0000,
        Mode_IN_FLOATING,   // = 0b 0000 0001,
        Mode_IPD,           // = 0b 0000 0010,
        Mode_IPU = 0x12,    // = 0b 0001 0010,
        Mode_Out_OD = 0x04, // = 0b 0000 0100,
        Mode_Out_PP,        // = 0b 0000 0101,
        Mode_AF_OD,         // = 0b 0000 0110,
        Mode_AF_PP,         // = 0b 0000 0111
    } Mode;
    typedef enum
    {
        Speed_10MHz = 1,
        Speed_2MHz,
        Speed_50MHz
    } Speed;

    void afConfig(const char *);
    void modeConfig(const char *, Mode m, Speed s = Speed_50MHz);
    void modeConfig(GPIO_TypeDef *, uint8_t p, Mode m, Speed s = Speed_50MHz);
    void extiConfig(const char *, void (*)(void), Mode = Mode_IPU);

    class GpioPin
    {
    private:
        uint16_t pin;
        uint16_t pinNum;
        GPIO_TypeDef *port;

    public:
        GpioPin(const char *, Mode =Mode::Mode_Out_PP, Speed = Speed_50MHz);
        // ~GpioPin();

        void set(void);
        void reset(void);
        // void setOnOff(bool);
        void flip();
        void setMode(Mode m, Speed s);
        // void setSpeed(GpioSpeed s);

        bool read(void);
        void operator=(bool s);
        void operator=(GpioPin &o);
        bool operator!(void);
        GpioPin &operator<<(bool);
        GpioPin &operator>>(bool &);
    };
} // namespace GPIO

#endif /* CD0E5873_EDE4_4337_A345_A2D0DE941A80 */
