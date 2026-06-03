// badprog.com
/**
 * @file    main.c
 * @brief   LED ring rotation with TIM2 hardware timer
 *
 * Bare metal C. No HAL, no CubeMX.
 *
 * TIM2 is configured to generate an interrupt every 500ms.
 * In the interrupt handler, the active LED rotates one position clockwise.
 *
 * LEDs on GPIOE:
 *   PE8=LD4(blue)  PE9=LD3(red)   PE10=LD5(orange) PE11=LD7(green)
 *   PE12=LD9(blue) PE13=LD10(red) PE14=LD8(orange) PE15=LD6(green)
 *
 * CPU clock: 8 MHz (default HSI, no PLL configured)
 * TIM2 clock: 8 MHz (on APB1, x1 when APB1 prescaler = 1)
 *
 * Timer formula:
 *   Period = (Prescaler + 1) * (ARR + 1) / TIM_clock
 *   500ms  = (7999 + 1)  * (499 + 1)  / 8 000 000
 *          = 8000 * 500 / 8 000 000
 *          = 0.5s ✓
 */

#include "stm32f303xc.h"

// Current active LED position (0..7 for PE8..PE15)
static volatile uint8_t led_pos = 0;

// ---------------------------------------------------------------------------
// LEDs init — PE8..PE15 as push-pull outputs
// ---------------------------------------------------------------------------
static void leds_init(void)
{
    RCC->AHBENR |= RCC_AHBENR_IOPEEN;

    GPIOE->MODER &= ~(0xFFFFUL << 16);
    GPIOE->MODER |=  (0x5555UL << 16);
}

// ---------------------------------------------------------------------------
// Turn on a single LED by position (0=PE8, 1=PE9, ... 7=PE15)
// ---------------------------------------------------------------------------
static void led_set(uint8_t pos)
{
    // Turn all LEDs off
    GPIOE->BSRR = (LED_ALL_PINS << 16);
    // Turn on the selected LED
    GPIOE->BSRR = (1UL << (8 + pos));
}

// ---------------------------------------------------------------------------
// TIM2 init — interrupt every 500ms
// CPU = 8 MHz, Prescaler = 7999, ARR = 499
// ---------------------------------------------------------------------------
static void tim2_init(void)
{
    // Enable TIM2 clock on APB1
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Prescaler: divide clock by (7999+1) = 8000
    // TIM2 tick frequency = 8 MHz / 8000 = 1 kHz
    TIM2->PSC = 7999;

    // Auto-reload: count up to 499, then reset and trigger interrupt
    // Period = (499+1) / 1 kHz = 500ms
    TIM2->ARR = 499;

    // Enable update interrupt (UIE)
    TIM2->DIER |= TIM_DIER_UIE;

    // Enable TIM2 in NVIC
    NVIC_EnableIRQ(TIM2_IRQn);

    // Start the timer (CEN = Counter ENable)
    TIM2->CR1 |= TIM_CR1_CEN;
}

// ---------------------------------------------------------------------------
// TIM2 interrupt handler — called every 500ms
// ---------------------------------------------------------------------------
void TIM2_IRQHandler(void)
{
    // Check update interrupt flag
    if (TIM2->SR & TIM_SR_UIF) {
        // Clear the flag
        TIM2->SR &= ~TIM_SR_UIF;

        // Rotate to next LED
        led_set(led_pos);
        led_pos = (led_pos + 1) % 8;
    }
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------
int main(void)
{
    leds_init();
    tim2_init();

    // Everything is handled by the interrupt — nothing to do here
    while (1) {
        __asm__("wfi");  // Wait For Interrupt — saves power
    }

    return 0;
}