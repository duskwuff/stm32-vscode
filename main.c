#include "project.h"
#include <stdint.h>

#include "gpio.h"
#include "systick.h"

extern void *__isr_vector[];

void SystemInit(void)
{
    // Here is where we would configure the system clocks.
    //
    // By default the system runs on the MSI RC oscillator @ 16 MHz...
    SysTick_Config(16 * 1000);
}

int main(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    (void)RCC->AHB1ENR;

    GPIO_CONFIGURE(
        // LED
        {
            .port = GPIO_PORT_C,
            .pins = GPIO_PIN_13,
            .mode = GPIO_MODE_OUTPUT,
        });

    for (;;)
    {
        GPIOC->BSRR = GPIO_BSRR_BR13;
        delay_ms(100);
        GPIOC->BSRR = GPIO_BSRR_BS13;
        delay_ms(100);
        GPIOC->BSRR = GPIO_BSRR_BR13;
        delay_ms(100);
        GPIOC->BSRR = GPIO_BSRR_BS13;
        delay_ms(500);
    }
}
