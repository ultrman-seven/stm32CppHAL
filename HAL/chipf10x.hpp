#ifndef F36D4273_B995_4D28_85CB_FEC0937E420E
#define F36D4273_B995_4D28_85CB_FEC0937E420E
#include "stdint.h"
#include "stm32f10x.h"

#define __RCC_GPIO_ENR *((uint32_t *)(RCC_BASE + 0x18))

extern const uint32_t __GPIO_PORT_BASEs[];
extern const uint32_t __GPIO_PORT_RCC_EN[];

extern const uint32_t __USART_BASEs[];
extern const uint32_t __USART_RCC_EN[];
extern uint32_t const __USART_RCC_ENR_BASEs[];

extern const uint32_t __DMA_BASEs[];
extern const uint32_t __DMA_RCC_EN[];
extern uint32_t const __DMA_RCC_ENR_BASEs[];

extern const uint32_t __ADC_BASEs[];
extern const uint32_t __ADC_RCC_EN[];
extern uint32_t const __ADC_RCC_ENR_BASEs[];
#endif /* F36D4273_B995_4D28_85CB_FEC0937E420E */
