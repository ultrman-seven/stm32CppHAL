#ifndef D0AD59C9_35A4_4733_A952_87050DEAAE67
#define D0AD59C9_35A4_4733_A952_87050DEAAE67
#include "chipf10x.hpp"
namespace TIM
{
    volatile uint32_t *pwm(uint8_t tim, uint8_t ch, const char *p = nullptr, const char *n = nullptr, uint32_t per = 7200, uint32_t psr = 1);
} // namespace TIM
#endif /* D0AD59C9_35A4_4733_A952_87050DEAAE67 */
