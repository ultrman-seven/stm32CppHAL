#include "wyTim.hpp"

namespace TIM
{
    volatile uint32_t *pwm(uint8_t tim, uint8_t ch, const char *p = nullptr, const char *n = nullptr, uint32_t per = 7200, uint32_t psr = 1);

    class TimBase
    {
    private:
        /* data */
    public:
        TimBase(uint8_t x, uint32_t p, uint32_t s);
        ~TimBase();
    };
} // namespace TIM

void (*timUpCallbacks[8])(void) = {nullptr};

namespace TIM
{
}

#define __TIMx_UP_IRQHandler(__TIM_X)

void TIM1_UP_IRQHandler(void) { __TIMx_UP_IRQHandler(1) }
void TIM8_UP_IRQHandler(void) { __TIMx_UP_IRQHandler(8) }

void TIM2_IRQHandler(void) { __TIMx_UP_IRQHandler(2) }
void TIM3_IRQHandler(void) { __TIMx_UP_IRQHandler(3) }
void TIM4_IRQHandler(void) { __TIMx_UP_IRQHandler(4) }
void TIM5_IRQHandler(void) { __TIMx_UP_IRQHandler(5) }
void TIM6_IRQHandler(void) { __TIMx_UP_IRQHandler(6) }
void TIM7_IRQHandler(void) { __TIMx_UP_IRQHandler(7) }
