#include "wyUsart.hpp"
#include "wyGpio.hpp"
#include "wySys.hpp"

using namespace USART;

#define USART_TxEndFlag(__USART) ((__USART)->SR & 0xffffff7f)

static inline uint32_t __UsartAfCfg(uint8_t n, const char *tx)
{
    switch (n)
    {
    case 1:
        if (*tx == 'b' || *tx == 'B')
            return 0x04;
        return 0x00;
    case 2:
        if (*tx == 'd' || *tx == 'D')
            return 0x08;
        return 0x00;
    case 3:
        if (*tx == 'c' || *tx == 'C')
            return 0x10;
        if (*tx == 'd' || *tx == 'D')
            return 0x30;
        return 0x00;
    default:
        return 0x00;
    }
    return 0x00;
}

Serial::Serial(uint8_t n, const char *rx, const char *tx, uint32_t bode)
{
    GPIO::modeConfig(rx, GPIO::Mode::Mode_IN_FLOATING);
    GPIO::modeConfig(tx, GPIO::Mode::Mode_AF_PP);

    AFIO->MAPR |= __UsartAfCfg(n, tx);

    (*(volatile uint32_t *)__USART_RCC_ENR_BASEs[n - 1]) |= __USART_RCC_EN[n - 1];
    this->uart = (USART_TypeDef *)__USART_BASEs[n - 1];

    // bode rate set
    double bodeDiv = (n == 1 ? __SysClockAPB2 : __SysClockAPB1) / 16 / bode;
    // double bodeDiv = 72000000 / bode / 16;
    uint32_t divM = bodeDiv;
    bodeDiv -= divM;
    bodeDiv *= 16;
    if ((bodeDiv - (int)bodeDiv) >= 0.5)
        bodeDiv += 1;
    uint32_t divF = bodeDiv;
    this->uart->BRR = (divM << 4) | divF;

    this->uart->CR1 = 0x200c;
    this->uart->CR2 = 0;
    this->uart->CR3 = 0;
}

uint8_t Serial::receiveByte(void) { return this->uart->DR & (uint8_t)0x00fF; }

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
Serial &Serial::operator>>(uint8_t &dat)
{
    dat = this->receiveByte();
    return *this;
}

void USART1_IRQHandler(void) {}
void USART2_IRQHandler(void) {}
void USART3_IRQHandler(void) {}

void UART4_IRQHandler(void) {}
void UART5_IRQHandler(void) {}
