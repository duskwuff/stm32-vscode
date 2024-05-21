#pragma once
#include <stdint.h>

struct GPIO_Config
{
    uint32_t pins : 16, od : 1, init : 1, mode : 2, speed : 2, pull : 2, afio : 4, port : 4;
};

void gpio_configure(const struct GPIO_Config *cfg);

#define GPIO_CONFIGURE(cfg...)                         \
    do                                                 \
    {                                                  \
        const struct GPIO_Config _pins[] = {cfg, {0}}; \
        gpio_configure(_pins);                         \
    } while (0)

#define GPIO_PIN_0 (1 << 0)
#define GPIO_PIN_1 (1 << 1)
#define GPIO_PIN_2 (1 << 2)
#define GPIO_PIN_3 (1 << 3)
#define GPIO_PIN_4 (1 << 4)
#define GPIO_PIN_5 (1 << 5)
#define GPIO_PIN_6 (1 << 6)
#define GPIO_PIN_7 (1 << 7)
#define GPIO_PIN_8 (1 << 8)
#define GPIO_PIN_9 (1 << 9)
#define GPIO_PIN_10 (1 << 10)
#define GPIO_PIN_11 (1 << 11)
#define GPIO_PIN_12 (1 << 12)
#define GPIO_PIN_13 (1 << 13)
#define GPIO_PIN_14 (1 << 14)
#define GPIO_PIN_15 (1 << 15)

#define GPIO_PORT_A 0
#define GPIO_PORT_B 1
#define GPIO_PORT_C 2
#define GPIO_PORT_D 3
#define GPIO_PORT_E 4
#define GPIO_PORT_F 5
#define GPIO_PORT_G 6
#define GPIO_PORT_H 7
#define GPIO_PORT_I 8
#define GPIO_PORT_J 9
#define GPIO_PORT_K 10
#define GPIO_PORT_L 11
#define GPIO_PORT_M 12
#define GPIO_PORT_N 13
#define GPIO_PORT_O 14
#define GPIO_PORT_P 15

#define GPIO_MODE_INPUT 0
#define GPIO_MODE_OUTPUT 1
#define GPIO_MODE_AFIO 2
#define GPIO_MODE_ANALOG 3

#define GPIO_SPEED_LOW 0
#define GPIO_SPEED_MED 1
#define GPIO_SPEED_HIGH 2
#define GPIO_SPEED_VERYHIGH 3

#define GPIO_PULL_NONE 0
#define GPIO_PULL_UP 1
#define GPIO_PULL_DOWN 2

#ifdef MOCK_GPIO_TYPEDEF
typedef struct
{
    uint32_t MODER;
    uint32_t OTYPER;
    uint32_t OSPEEDR;
    uint32_t PUPDR;
    uint32_t BSRR;
    uint32_t AFR[2];
} GPIO_TypeDef;
#endif /* MOCK_GPIO_TYPEDEF */
