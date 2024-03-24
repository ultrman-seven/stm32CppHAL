#include "wyADC.hpp"
#include "wyDMA.hpp"
#include "wyGpio.hpp"

const uint8_t ADC_DMA_CH[] = {0x11};
// const uint32_t __ADC_BASEs[] = {ADC1_BASE, ADC2_BASE, ADC3_BASE};

const uint8_t __ADC_PING_RULE[] = {0, 8, 10};
static inline uint32_t __AdcPin2ChannelAndModeCfg(const char *p)
{
    char pn = *p;
    GPIO::modeConfig(p, GPIO::Mode_AIN);
    // uint8_t pin = 0;
    if (pn >= 'a' && pn <= 'z')
        pn -= 'a';
    else
        pn -= 'A';
    // while (*++p)
    // {
    //     pin *= 10;
    //     pin += *p - '0';
    // }
    // return __ADC_PING_RULE[pn] + pin;
    return __ADC_PING_RULE[pn] + p[1] - '0';
}

namespace ADC
{
    void dmaWay(uint8_t ADCx, uint8_t n, const char *pin[], uint32_t add)
    {
        --ADCx;
        DMA::configUnit cfg;
        ADC_TypeDef *adc = (ADC_TypeDef *)__ADC_BASEs[ADCx];

        uint32_t sqVal = 0;
        uint8_t cnt = 0;
        uint8_t bitCnt = 0;

        *(volatile uint32_t *)__ADC_RCC_ENR_BASEs[ADCx] |= __ADC_RCC_EN[ADCx];

        adc->CR1 = 0x00000100;
        adc->CR2 = 0x000e0102;

        adc->SMPR1 = 0x00924924;
        adc->SMPR2 = 0x24924924;

        cfg.memoryAddr = add;
        cfg.peripheralAddr = (uint32_t)(&(adc->DR));

        cfg.bufferSize = n;

        cfg.channelConfig.m2m = 0;
        cfg.channelConfig.dirMem2Per = 0;
        cfg.channelConfig.en = 1;
        cfg.channelConfig.circularMode = 1;
        cfg.channelConfig.mByteSize = DMA_ConfigByte2_bit16;
        cfg.channelConfig.pByteSize = DMA_ConfigByte2_bit16;
        cfg.channelConfig.mInc = 1;
        cfg.channelConfig.pInc = 0;

        cfg.config((ADC_DMA_CH[ADCx] >> 4) & 0x0f, ADC_DMA_CH[ADCx] & 0x0f);

        RCC->CFGR |= 0xc000;

        sqVal = 0;
        cnt = 0;
        bitCnt = 0;
        while (cnt < 6 && cnt < n)
        {
            sqVal |= (__AdcPin2ChannelAndModeCfg(pin[cnt++]) << bitCnt);
            bitCnt += 5;
        }
        adc->SQR3 = sqVal;

        sqVal = 0;
        bitCnt = 0;
        while (cnt < 12 && cnt < n)
        {
            sqVal |= (__AdcPin2ChannelAndModeCfg(pin[cnt++]) << bitCnt);
            bitCnt += 5;
        }
        adc->SQR2 = sqVal;

        sqVal = n - 1;
        sqVal <<= 20;
        bitCnt = 0;
        while (cnt < 16 && cnt < n)
        {
            sqVal |= (__AdcPin2ChannelAndModeCfg(pin[cnt++]) << bitCnt);
            bitCnt += 5;
        }
        adc->SQR1 = sqVal;

        adc->CR2 |= 0x01;

        adc->CR2 |= 0x04;
        while (adc->CR2 & 0x04)
            ;
        adc->CR2 |= 0x00500000;
    }
} // namespace ADC
