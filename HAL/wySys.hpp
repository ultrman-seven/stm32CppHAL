#ifndef E3C042EF_FDF2_4DCE_888D_4FD7663D55FF
#define E3C042EF_FDF2_4DCE_888D_4FD7663D55FF
#include "stdint.h"
extern uint32_t __SysClockSYS;
extern uint32_t __SysClockAHB;
extern uint32_t __SysClockAPB1;
extern uint32_t __SysClockAPB2;
namespace wySys
{
    // struct ClockInfo
    // {
    //     uint32_t sys;
    //     uint32_t ahb;
    //     uint32_t apb1;
    //     uint32_t apb2;
    // };
    // extern ClockInfo clock;
    // extern uint32_t &clockSys;
    // extern uint32_t &clockAHB;
    // extern uint32_t &clockAPB1;
    // extern uint32_t &clockAPB2;
    void deleteLoop(void (*f)(void));
    // void delayInit(void);
    void runLoopFunctions(void);
    void throwFunc2Loop(void (*)(void));
    void delayMs(uint32_t ms);
    uint32_t getTimeStamp(void);
} // namespace wySys
namespace sys = wySys;
#endif /* E3C042EF_FDF2_4DCE_888D_4FD7663D55FF */
