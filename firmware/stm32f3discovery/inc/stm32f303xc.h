// badprog.com
#ifndef STM32F303XC_H
#define STM32F303XC_H

#include <stdint.h>

typedef volatile uint32_t reg32_t;

// ---------------------------------------------------------------------------
// Bus base addresses
// ---------------------------------------------------------------------------
#define PERIPH_BASE         (0x40000000UL)
#define APB1PERIPH_BASE     (PERIPH_BASE)
#define AHB1PERIPH_BASE     (PERIPH_BASE + 0x00020000UL)
#define AHB2PERIPH_BASE     (PERIPH_BASE + 0x08000000UL)

// ---------------------------------------------------------------------------
// RCC — Reset and Clock Control
// ---------------------------------------------------------------------------
#define RCC_BASE            (AHB1PERIPH_BASE + 0x00001000UL)

typedef struct {
    reg32_t CR;
    reg32_t CFGR;
    reg32_t CIR;
    reg32_t APB2RSTR;
    reg32_t APB1RSTR;
    reg32_t AHBENR;
    reg32_t APB2ENR;
    reg32_t APB1ENR;
    reg32_t BDCR;
    reg32_t CSR;
    reg32_t AHBRSTR;
    reg32_t CFGR2;
    reg32_t CFGR3;
} RCC_TypeDef;

#define RCC                 ((RCC_TypeDef *) RCC_BASE)

#define RCC_AHBENR_IOPEEN   (1UL << 21)
#define RCC_APB1ENR_TIM2EN  (1UL << 0)

// ---------------------------------------------------------------------------
// GPIO
// ---------------------------------------------------------------------------
#define GPIOE_BASE          (AHB2PERIPH_BASE + 0x00001000UL)

typedef struct {
    reg32_t MODER;
    reg32_t OTYPER;
    reg32_t OSPEEDR;
    reg32_t PUPDR;
    reg32_t IDR;
    reg32_t ODR;
    reg32_t BSRR;
    reg32_t LCKR;
    reg32_t AFR[2];
    reg32_t BRR;
} GPIO_TypeDef;

#define GPIOE               ((GPIO_TypeDef *) GPIOE_BASE)

// LEDs on GPIOE PE8..PE15
#define LED_ALL_PINS        (0xFFUL << 8)

// ---------------------------------------------------------------------------
// TIM2 — General purpose timer
// ---------------------------------------------------------------------------
#define TIM2_BASE           (APB1PERIPH_BASE + 0x00000000UL)

typedef struct {
    reg32_t CR1;
    reg32_t CR2;
    reg32_t SMCR;
    reg32_t DIER;
    reg32_t SR;
    reg32_t EGR;
    reg32_t CCMR1;
    reg32_t CCMR2;
    reg32_t CCER;
    reg32_t CNT;
    reg32_t PSC;
    reg32_t ARR;
    reg32_t RES1;
    reg32_t CCR1;
    reg32_t CCR2;
    reg32_t CCR3;
    reg32_t CCR4;
    reg32_t RES2;
    reg32_t DCR;
    reg32_t DMAR;
    reg32_t OR;
} TIM_TypeDef;

#define TIM2                ((TIM_TypeDef *) TIM2_BASE)

#define TIM_CR1_CEN         (1UL << 0)   // Counter enable
#define TIM_DIER_UIE        (1UL << 0)   // Update interrupt enable
#define TIM_SR_UIF          (1UL << 0)   // Update interrupt flag

// ---------------------------------------------------------------------------
// NVIC — Nested Vectored Interrupt Controller
// ---------------------------------------------------------------------------
#define NVIC_BASE           (0xE000E100UL)

typedef struct {
    reg32_t ISER[8];   // Interrupt Set Enable
    reg32_t RESERVED0[24];
    reg32_t ICER[8];   // Interrupt Clear Enable
    reg32_t RESERVED1[24];
    reg32_t ISPR[8];   // Interrupt Set Pending
    reg32_t RESERVED2[24];
    reg32_t ICPR[8];   // Interrupt Clear Pending
} NVIC_TypeDef;

#define NVIC                ((NVIC_TypeDef *) NVIC_BASE)

// TIM2 IRQ number on STM32F303 = 28
#define TIM2_IRQn           28

static inline void NVIC_EnableIRQ(uint32_t irq)
{
    NVIC->ISER[irq >> 5] = (1UL << (irq & 0x1F));
}

#endif // STM32F303XC_H