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
};

void gpio_configure(const struct GPIO_Config *cfg)
{
    uint32_t mode, otype, ospeed, pupd, afrl, afrh, bsrr;

    while (cfg->pins != 0)
    {
        uint32_t port = cfg->port;
        GPIO_TypeDef *gpio = gpio_periphs[port];

        mode = gpio->MODER;
        otype = gpio->OTYPER;
        ospeed = gpio->OSPEEDR;
        pupd = gpio->PUPDR;
        afrl = gpio->AFR[0];
        afrh = gpio->AFR[1];
        bsrr = 0;

    same_port:;
        uint32_t pins1 = cfg->pins;

        otype &= ~pins1;
        otype += pins1 * cfg->od;

        bsrr |= cfg->init ? pins1 : (pins1 << 16);

        // Expand 16 bits to 32, inserting zeroes in alternate bits
        uint32_t pins2 = pins1;
        pins2 |= pins2 << 8;
        pins2 &= 0x00ff00ff;
        pins2 |= pins2 << 4;
        pins2 &= 0x0f0f0f0f;
        pins2 |= pins2 << 2;
        pins2 &= 0x33333333;
        pins2 |= pins2 << 1;
        pins2 &= 0x55555555;

        mode &= ~(pins2 * 0x3);
        mode += pins2 * cfg->mode;

        ospeed &= ~(pins2 * 0x3);
        ospeed += pins2 * cfg->speed;

        pupd &= ~(pins2 * 0x3);
        pupd += pins2 * cfg->pull;

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

        afrl &= ~(pins4_lo * 0xf);
        afrl += pins4_lo * cfg->afio;

        // Same as above, but with the upper 8 bits.
        uint32_t pins4_hi = pins1 >> 8;
        pins4_hi |= pins4_hi << 12;
        pins4_hi &= 0x000f000f;
        pins4_hi |= pins4_hi << 6;
        pins4_hi &= 0x03030303;
        pins4_hi |= pins4_hi << 3;
        pins4_hi &= 0x11111111;

        afrh &= ~(pins4_hi * 0xf);
        afrh += pins4_hi * cfg->afio;

        // Move to the next config...
        cfg++;

        // If the next operation involves the same port, merge the operations.
        if (cfg->pins != 0 && cfg->port == port)
        {
            goto same_port;
        }

        // Write values back to the peripheral.
        //
        // Order of operations matches that used by ST's HAL, with BSRR added
        // after PUPDR.
        gpio->OSPEEDR = ospeed;
        gpio->OTYPER = otype;
        gpio->PUPDR = pupd;
        gpio->BSRR = bsrr;
        gpio->AFR[0] = afrl;
        gpio->AFR[1] = afrh;
        gpio->MODER = mode;
    }
}
