#include "wyUsart.hpp"
#include "wyGpio.hpp"
#include "wySys.hpp"
#include "queue"

using namespace USART;

static inline uint32_t __UsartAfCfg(uint8_t n, const char *txOrRx)
{
    switch (n)
    {
    case 1:
        if (*txOrRx == 'b' || *txOrRx == 'B')
            return 0x04;
        return 0x00;
    case 2:
        if (*txOrRx == 'd' || *txOrRx == 'D')
            return 0x08;
        return 0x00;
    case 3:
        if (*txOrRx == 'c' || *txOrRx == 'C')
            return 0x10;
        if (*txOrRx == 'd' || *txOrRx == 'D')
            return 0x30;
        return 0x00;
    default:
        return 0x00;
    }
    return 0x00;
}

Serial::Serial(uint8_t n, const char *rx, const char *tx, uint32_t baud)
{
    (*(volatile uint32_t *)__USART_RCC_ENR_BASEs[n - 1]) |= __USART_RCC_EN[n - 1];
    this->uart = (USART_TypeDef *)__USART_BASEs[n - 1];

    double baudDiv = (n == 1 ? __SysClockAPB2 : __SysClockAPB1) / 16 / baud;
    uint32_t divM = baudDiv;
    baudDiv -= divM;
    baudDiv *= 16;
    if ((baudDiv - (int)baudDiv) >= 0.5)
        baudDiv += 1;
    uint32_t divF = baudDiv;
    this->uart->BRR = (divM << 4) | divF;

    this->uart->CR1 = 0x2000;
    this->uart->CR2 = 0;
    this->uart->CR3 = 0;

    if (rx != nullptr)
    {
        GPIO::modeConfig(rx, GPIO::Mode::Mode_IN_FLOATING);
        this->uart->CR1 |= 0x0004;
    }
    if (tx != nullptr)
    {
        GPIO::modeConfig(tx, GPIO::Mode::Mode_AF_PP);
        this->uart->CR1 |= 0x0008;
    }

    AFIO->MAPR |= __UsartAfCfg(n, tx == nullptr ? rx : tx);

    // baud set
}

uint8_t Serial::receiveByte(void) { return this->uart->DR & (uint8_t)0x00fF; }

#define USART_TxEndFlag(__USART) ((__USART)->SR & 0xffffff7f)
void Serial::sendByte(uint8_t dat)
{
    this->uart->DR = (dat & (uint8_t)0xFF);
    while (!USART_TxEndFlag(this->uart))
        ;
}

void Serial::sendByte(uint8_t *dat, uint8_t len)
{
    while (len--)
    {
        this->uart->DR = (*dat++ & (uint8_t)0xFF);
        while (!USART_TxEndFlag(this->uart))
            ;
    }
}

void Serial::putChar(uint8_t dat)
{
    this->uart->DR = (dat & (uint8_t)0xFF);
    while (!USART_TxEndFlag(this->uart))
        ;
}

#if 0
Serial &Serial::operator<<(uint8_t dat)
{
    this->uart->DR = (dat & (uint16_t)0x00FF);
    while (!USART_TxEndFlag(this->uart))
        ;
    return *this;
}

#include "stdio.h"
char __wyUsartStrTmp[20];
Serial &Serial::operator<<(int32_t num)
{
    sprintf(__wyUsartStrTmp, "%d", num);
    this->operator<<(__wyUsartStrTmp);
    return *this;
}
Serial &Serial::operator<<(float num)
{
    sprintf(__wyUsartStrTmp, "%f", num);
    this->operator<<(__wyUsartStrTmp);
    return *this;
}
Serial &Serial::operator<<(const char c)
{
    this->uart->DR = (c & (uint16_t)0x00FF);
    while (!USART_TxEndFlag(this->uart))
        ;
    return *this;
}
Serial &Serial::operator<<(const char *s)
{
    while (*s)
    {
        this->uart->DR = (*s++ & (uint8_t)0x00fF);
        while (!USART_TxEndFlag(this->uart))
            ;
    }
    return *this;
}
Serial &Serial::operator<<(char *s)
{
    while (*s)
    {
        this->uart->DR = (*s++ & (uint16_t)0x00FF);
        while (!USART_TxEndFlag(this->uart))
            ;
    }
    return *this;
}
#endif

Serial &Serial::operator>>(uint8_t &dat)
{
    dat = this->receiveByte();
    return *this;
}

void (*__USART_RxIRQ_Callbacks[5])(uint8_t);
std::queue<uint8_t> __USART_fifo[5];

void USART1_IRQHandler(void) {}
void USART2_IRQHandler(void) {}
void USART3_IRQHandler(void) {}

void UART4_IRQHandler(void) {}
void UART5_IRQHandler(void) {}
