#include "project.h"
#include "gpio.h"

GPIO_TypeDef *const gpio_periphs[16] = {
    [0] = GPIOA,
#ifdef GPIOB
    [1] = GPIOB,
#endif
#ifdef GPIOC
    [2] = GPIOC,
#endif
#ifdef GPIOD
    [3] = GPIOD,
#endif
#ifdef GPIOE
    [4] = GPIOE,
#endif
#ifdef GPIOF
    [5] = GPIOF,
#endif
#ifdef GPIOG
    [6] = GPIOG,
#endif
#ifdef GPIOH
    [7] = GPIOH,
#endif
#ifdef GPIOI
    [8] = GPIOI,
#endif
#ifdef GPIOJ
    [9] = GPIOJ,
#endif
#ifdef GPIOK
    [10] = GPIOK,
#endif
#ifdef GPIOL
    [11] = GPIOL,
#endif
#ifdef GPIOM
    [12] = GPIOM,
#endif
#ifdef GPION
    [13] = GPION,
#endif
#ifdef GPIOO
    [14] = GPIOO,
#endif
#ifdef GPIOP
    [15] = GPIOP,
#endif
    0};

void gpio_configure(const struct GPIO_Config *cfg)
{
    uint32_t crl, crh, bsrr;

    const uint32_t mode_map[4] = {
        [GPIO_MODE_INPUT] = 0b0100,  // floating input
        [GPIO_MODE_OUTPUT] = 0b0000, // speed in lower bits
        [GPIO_MODE_AFIO] = 0b1000,   // ditto
        [GPIO_MODE_ANALOG] = 0b0000,
    };

    const uint32_t speed_map[4] = {
        [GPIO_SPEED_LOW] = 0b0010,      // 2 MHz
        [GPIO_SPEED_MED] = 0b0001,      // 10 MHz
        [GPIO_SPEED_HIGH] = 0b0011,     // 50 MHz
        [GPIO_SPEED_VERYHIGH] = 0b0011, // same
    };

    while (cfg->pins != 0)
    {
        uint32_t port = cfg->port;
        GPIO_TypeDef *gpio = gpio_periphs[port];

        crl = gpio->CRL;
        crh = gpio->CRH;
        bsrr = 0;

    same_port:;
        uint32_t pins1 = cfg->pins;

        // Calculate cnf/mode
        uint32_t cnf_mode = mode_map[cfg->mode] | speed_map[cfg->speed];

        if (cnf_mode & 0b0011)
        { // output
            if (cfg->od)
            {
                cnf_mode |= 0b0100;
            }
            // TODO: open drain + pull? is that a thing on STM32F1?
        }
        else if (cfg->pull)
        {                       // input with pu/pd
            cnf_mode += 0b0100; // 0100 -> 1000
            if (cfg->pull == GPIO_PULL_UP)
                bsrr = GPIO_BSRR_BS0 * pins1;
            else
                bsrr = GPIO_BSRR_BR0 * pins1;
        }

        // Expand 8 bits to 32.
        //
        // Surprisingly, we don't need to extract the lower byte first; the
        // high bits are zeroed during the first shift/mask step.
        uint32_t pins4_lo = pins1;
        pins4_lo |= pins4_lo << 12;
        pins4_lo &= 0x000f000f;
        pins4_lo |= pins4_lo << 6;
        pins4_lo &= 0x03030303;
        pins4_lo |= pins4_lo << 3;
        pins4_lo &= 0x11111111;

        crl &= ~(0xf * pins4_lo);
        crl |= cnf_mode * pins4_lo;

        // Same as above, but with the upper 8 bits.
        uint32_t pins4_hi = pins1 >> 8;
        pins4_hi |= pins4_hi << 12;
        pins4_hi &= 0x000f000f;
        pins4_hi |= pins4_hi << 6;
        pins4_hi &= 0x03030303;
        pins4_hi |= pins4_hi << 3;
        pins4_hi &= 0x11111111;

        crh &= ~(0xf * pins4_hi);
        crh |= cnf_mode * pins4_hi;

        // Move to the next config...
        cfg++;

        // If the next operation involves the same port, merge the operations.
        if (cfg->pins != 0 && cfg->port == port)
        {
            goto same_port;
        }

        // Write values back to the peripheral.
        gpio->CRL = crl;
        gpio->CRH = crh;
        gpio->BSRR = bsrr;
    }
}
