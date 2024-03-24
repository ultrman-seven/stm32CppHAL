#include "wyGpio.hpp"
#include "chipf10x.hpp"

using namespace GPIO;

GpioPin::GpioPin(const char *name, Mode m, Speed s)
{
    char p = *name;

    p -= ((p >= 'a' && p <= 'z') ? 'a' : 'A');
    this->port = (GPIO_TypeDef *)__GPIO_PORT_BASEs[p];
    this->pinNum = 0;
    while (*++name)
    {
        this->pinNum *= 10;
        this->pinNum += *name - '0';
    }
    this->pin = (0x01) << (this->pinNum);

    __RCC_GPIO_ENR |= __GPIO_PORT_RCC_EN[p];
    // setMode(m, s);
    modeConfig(this->port, this->pinNum, m, s);
}

void GpioPin::setMode(Mode m, Speed s)
{
    modeConfig(this->port, this->pinNum, m, s);
    // uint32_t mark1, mark2;
    // uint8_t pn = this->pinNum;
    // if (pn >= 8)
    //     pn -= 8;
    // pn <<= 2;
    // mark1 = (0x0f << pn);
    // mark1 = ~mark1;

    // mark2 = (m & 0x04) ? s : 0;
    // mark2 |= ((m & 0x03) << 2);
    // mark2 <<= pn;

    // if (pin >= 0x0100)
    // {
    //     this->port->CRH &= mark1;
    //     this->port->CRH |= mark2;
    // }
    // else
    // {
    //     this->port->CRL &= mark1;
    //     this->port->CRL |= mark2;
    // }
}

GpioPin &GpioPin::operator<<(bool s)
{
    if (s)
        this->port->BSRR |= this->pin;
    else
        this->port->BRR |= this->pin;
    return *this;
}
GpioPin &GpioPin::operator>>(bool &s)
{
    s = (this->port->IDR & this->pin);
    return *this;
}

void GpioPin::set(void) { this->port->BSRR = this->pin; }
void GpioPin::reset(void) { this->port->BRR = this->pin; }
void GpioPin::flip(void) { this->operator=(!((this->port->ODR) & (this->pin))); }
bool GpioPin::read(void) { return this->port->IDR & this->pin; }
// void GpioPin::operator=(bool s) { s ? (this->port->ODR |= this->pin) : (this->port->ODR &= (~this->pin)); }
void GpioPin::operator=(bool s) { s ? this->set() : this->reset(); }
void GpioPin::operator=(GPIO::GpioPin &o) { this->operator=(o.read()); }
bool GpioPin::operator!(void) { return !(this->port->IDR & this->pin); }

void (*__GPIO_EXTI_Callbacks[16])(void) = {nullptr};

namespace GPIO
{
    void modeConfig(const char *name, Mode m,Speed s)
    {
        char p = *name;
        // this->port = (GPIO_TypeDef *)((p >= 'a' && p <= 'z') ? __GPIO_PORT_BASEs[p - 'a'] : __GPIO_PORT_BASEs[p - 'A']);
        p -= ((p >= 'a' && p <= 'z') ? 'a' : 'A');
        auto port = (GPIO_TypeDef *)__GPIO_PORT_BASEs[p];
        uint8_t pinNum = 0;
        while (*++name)
        {
            pinNum *= 10;
            pinNum += *name - '0';
        }

        __RCC_GPIO_ENR |= __GPIO_PORT_RCC_EN[p];

        modeConfig(port, pinNum, m, s);
    }

    void modeConfig(GPIO_TypeDef *port, uint8_t pinNum, Mode m, Speed s)
    {
        uint32_t mark1, mark2;
        uint8_t pn = pinNum;
        if (pn >= 8)
            pn -= 8;
        pn <<= 2;
        mark1 = (0x0f << pn);
        mark1 = ~mark1;

        mark2 = (m & 0x04) ? s : 0;
        mark2 |= ((m & 0x03) << 2);
        mark2 <<= pn;

        if (pinNum >= 8)
        {
            port->CRH &= mark1;
            port->CRH |= mark2;
        }
        else
        {
            port->CRL &= mark1;
            port->CRL |= mark2;
        }

        if (m == Mode_IPU)
            port->BSRR = (0x01 << pinNum);
        if (m == Mode_IPD)
            port->BRR = (0x01 << pinNum);
    }

    void extiConfig(const char *name, void (*f)(void),Mode m)
    {
        uint32_t line;
        char p = *name;
        p -= ((p >= 'a' && p <= 'z') ? 'a' : 'A');
        auto port = (GPIO_TypeDef *)__GPIO_PORT_BASEs[p];
        uint8_t pinNum = 0;
        while (*++name)
        {
            pinNum *= 10;
            pinNum += *name - '0';
        }

        __RCC_GPIO_ENR |= __GPIO_PORT_RCC_EN[p];

        modeConfig(port, pinNum, m);

        __GPIO_EXTI_Callbacks[pinNum] = f;

        line = 0x01 << pinNum;
        EXTI->IMR |= line;
        EXTI->EMR &= ~line;

        if (m == Mode_IPU)
            EXTI->FTSR |= line;
        // if (m == GPIO_Mode_IPD)
        else
            EXTI->RTSR |= line;

        volatile uint32_t *AFIO_EXTIR = (volatile uint32_t *)(AFIO_BASE + 0x08 + 0x04 * (pinNum >> 2));
        uint32_t AFIO_CFG = p;
        AFIO_CFG <<= (pinNum % 4) * 4;
        *AFIO_EXTIR |= AFIO_CFG;

        // nvic
        if (pinNum < 5)
            NVIC_EnableIRQ((IRQn)(EXTI0_IRQn + pinNum));
        else if (pinNum < 10)
            NVIC_EnableIRQ(EXTI9_5_IRQn);
        else
            NVIC_EnableIRQ(EXTI15_10_IRQn);
    }
}

#define __EXTI_LINE0 (0x0001)
#define __EXTI_LINE1 (0x0002)
#define __EXTI_LINE2 (0x0004)
#define __EXTI_LINE3 (0x0008)
#define __EXTI_LINE4 (0x0010)
#define __EXTI_LINE5 (0x0020)
#define __EXTI_LINE6 (0x0040)
#define __EXTI_LINE7 (0x0080)

#define __EXTI_LINE8 (0x0100)
#define __EXTI_LINE9 (0x0200)
#define __EXTI_LINE10 (0x0400)
#define __EXTI_LINE11 (0x0800)
#define __EXTI_LINE12 (0x1000)
#define __EXTI_LINE13 (0x2000)
#define __EXTI_LINE14 (0x4000)
#define __EXTI_LINE15 (0x8000)

#define __EXTIx_IRQHandler(__EXTIx)                        \
    if (EXTI->PR & __EXTI_LINE##__EXTIx)                   \
        if (EXTI->IMR & __EXTI_LINE##__EXTIx)              \
        {                                                  \
            if (__GPIO_EXTI_Callbacks[__EXTIx] != nullptr) \
                __GPIO_EXTI_Callbacks[__EXTIx]();          \
            EXTI->PR = __EXTI_LINE##__EXTIx;               \
        }

void EXTI0_IRQHandler(void) { __EXTIx_IRQHandler(0) }
void EXTI1_IRQHandler(void) { __EXTIx_IRQHandler(1) }
void EXTI2_IRQHandler(void) { __EXTIx_IRQHandler(2) }
void EXTI3_IRQHandler(void) { __EXTIx_IRQHandler(3) }
void EXTI4_IRQHandler(void) { __EXTIx_IRQHandler(4) }

void EXTI9_5_IRQHandler(void)
{
    __EXTIx_IRQHandler(5);
    __EXTIx_IRQHandler(6);
    __EXTIx_IRQHandler(7);
    __EXTIx_IRQHandler(8);
    __EXTIx_IRQHandler(9);
}

void EXTI15_10_IRQHandler(void)
{
    __EXTIx_IRQHandler(10);
    __EXTIx_IRQHandler(11);
    __EXTIx_IRQHandler(12);
    __EXTIx_IRQHandler(13);
    __EXTIx_IRQHandler(14);
    __EXTIx_IRQHandler(15);
}
