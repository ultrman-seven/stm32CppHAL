#include "wyDMA.hpp"

namespace DMA
{
    void stop(uint8_t n, uint8_t channel)
    {
        ((DMA_Channel_TypeDef *)(__DMA_BASEs[n - 1] + 0x08 + 20 * (channel - 1)))->CCR &= 0xfffffffe;
    }
    void start(uint8_t n, uint8_t channel)
    {
        ((DMA_Channel_TypeDef *)(__DMA_BASEs[n - 1] + 0x08 + 20 * (channel - 1)))->CCR |= 0x01;
    }
}

using namespace DMA;

configUnit::configUnit() : __valOfChannelConfig(0), bufferSize(1) {}

// typedef struct
// {
//     __IO uint32_t ISR;
//     __IO uint32_t IFCR;
//     struct
//     {
//         __IO uint32_t CCR;
//         __IO uint32_t CNDTR;
//         __IO uint32_t CPAR;
//         __IO uint32_t CMAR;
//         __IO uint32_t __;
//     } channel[7];
// } __wyDMA_Struct;

void configUnit::config(uint8_t n, uint8_t channel)
{
    // DMA_TypeDef *dma = (DMA_TypeDef *)__DMA_BASEs[n];
    --n;
    DMA_Channel_TypeDef *ch = (DMA_Channel_TypeDef *)(__DMA_BASEs[n] + 0x08 + 20 * (channel - 1));

    *(volatile uint32_t *)__DMA_RCC_ENR_BASEs[n] |= __DMA_RCC_EN[n];
    this->channelConfig.en = 0;
    ch->CCR = this->__valOfChannelConfig;
    ch->CMAR = this->memoryAddr;
    ch->CPAR = this->peripheralAddr;
    ch->CNDTR = this->bufferSize;
    ch->CCR |= 0x01;
}

// void DMA1_Channel1_IRQHandler(void)
// {
// }
