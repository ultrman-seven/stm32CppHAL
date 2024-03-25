#include "wyUsart.hpp"
#include "wyDMA.hpp"

#define DATA_SIZE 100
int main()
{
    USART::Serial com(1, "a10", "a9", 115200);
    int t;
    int dataA[DATA_SIZE];
    int dataB[DATA_SIZE];

    int cnt = DATA_SIZE;

    while (cnt--)
        dataA[cnt] = cnt;

    DMA::configUnit dc;
    dc.bufferSize = DATA_SIZE;
    dc.memoryAddr = (uint32_t)dataB;
    dc.peripheralAddr = (uint32_t)dataA;

    dc.channelConfig.circularMode = 0;
    dc.channelConfig.dirMem2Per = 0;
    dc.channelConfig.m2m = 1;
    dc.channelConfig.mByteSize = DMA_ConfigByte4_bit32;
    dc.channelConfig.pByteSize = DMA_ConfigByte4_bit32;
    dc.channelConfig.mInc = 1;
    dc.channelConfig.pInc = 1;

    com << "DMA test.\ndataB:\n";
    dc.config();

    cnt = 0;
    while (cnt < DATA_SIZE)
        com << dataB[cnt++] << "\n";

    while (1)
        ;
    return 0;
}
