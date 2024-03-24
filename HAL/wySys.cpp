#include "stdint.h"
#include "list"
#include "chipf10x.h"
#include "wySys.hpp"

std::list<void (*)(void)> __mainLoopFuncs;
// uint64_t __sysMsTimeStamp;
uint32_t __sysMsTimeStamp;
volatile uint32_t __sysDelayCnt;

void SysTick_Handler(void)
{
    ++__sysMsTimeStamp;
    if (__sysDelayCnt)
        --__sysDelayCnt;
}

uint32_t __SysClockSYS = 72000000;
uint32_t __SysClockAHB = 72000000;
uint32_t __SysClockAPB1 = 36000000;
uint32_t __SysClockAPB2 = 72000000;

extern "C"
{
    // void SystemInit();
    void SystemInitWy(void)
    {
        SystemInit();
        __sysMsTimeStamp = 0;
        __sysDelayCnt = 0;
        SysTick_Config(72000);
        SystemCoreClockUpdate();

        __SysClockSYS = 72000000;
        __SysClockAHB = __SysClockSYS;
        __SysClockAPB1 = __SysClockAHB >> 1;
        __SysClockAPB2 = __SysClockAHB;
    }
}

#include "chipf10x.h"
void (*__sysFoo)(void);
bool __funcListRmvIf(void (*a)(void))
{
    return a == __sysFoo;
}
namespace wySys
{
    // extern ClockInfo clock = {__SysClockSYS, __SysClockAHB, __SysClockAPB1, __SysClockAPB2};
    // ClockInfo clock;
    // uint32_t &clockSys = __SysClockSYS;
    // uint32_t &clockAHB = __SysClockAHB;
    // uint32_t &clockAPB1 = __SysClockAPB1;
    // uint32_t &clockAPB2 = __SysClockAPB2;

    uint32_t getTimeStamp(void)
    {
        return __sysMsTimeStamp;
    }
    void throwFunc2Loop(void (*f)(void))
    {
        __mainLoopFuncs.push_back(f);
    }
    void deleteLoop(void (*f)(void))
    {
        __sysFoo = f;
        __mainLoopFuncs.remove_if(__funcListRmvIf);
    }
    void delayMs(uint32_t ms)
    {
        __sysDelayCnt = ms;
        while (__sysDelayCnt)
            ;
    }
    // void delayInit(void)
    // {
    //     // SystemCoreClockUpdate();
    //     // SysTick_Config(SystemCoreClock / 1000);
    //     __sysMsTimeStamp = 0;
    //     __sysDelayCnt = 0;
    //     SysTick_Config(72000);
    //     // NVIC_SetPriority(SysTick_IRQn, 0);
    // }
    void runLoopFunctions(void)
    {
        for (auto i : __mainLoopFuncs)
            if (i != nullptr)
                i();
    }
}
