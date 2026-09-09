#include <stdbool.h>
#include <stdint.h>
#include "stm32g474_gpio.h"
#include "vendor/Device/ST/STM32G4xx/Include/stm32g474xx.h"


static void gpio_init_mode(GPIO_TypeDef *port_reg, uint8_t pin, gpio_mode_t mode)
{
    port_reg->MODER =
        ((port_reg->MODER & ~(3UL << (pin * 2))) | ((uint32_t)mode << (pin * 2)));
}


static void gpio_init_output_type(GPIO_TypeDef *port_reg, uint8_t pin,
                                  gpio_output_type_t output_type)
{
    port_reg->OTYPER =
        (port_reg->OTYPER & (~(1UL << pin)) | ((uint32_t)output_type << pin));
}


static void gpio_init_output_speed(GPIO_TypeDef *port_reg, uint8_t pin,
                                   gpio_speed_t speed)
{
    port_reg->OSPEEDR =
        ((port_reg->OSPEEDR & ~(3UL << (pin * 2))) | ((uint32_t)speed << (pin * 2)));
}


static void gpio_init_pull(GPIO_TypeDef *port_reg, uint8_t pin, gpio_pull_t pull)
{
    port_reg->PUPDR =
        ((port_reg->PUPDR & ~(3UL << (pin * 2))) | ((uint32_t)pull << (pin * 2)));
}


static void gpio_init_af(GPIO_TypeDef *port_reg, uint8_t pin, gpio_af_t af)
{
    uint8_t idx;
    uint8_t shift;

    if (8 > pin)
    {
        idx = 0;
        shift = pin * 4U;
    }
    else if (16 > pin)
    {
        idx = 1;
        shift = (pin - 8U) * 4U;
    }
    else
    {
        return;
    }

    port_reg->AFR[idx] = ((port_reg->AFR & ~(15U << shift)) | ((uint32_t)af << shift));
}


static void gpio_init_lock(GPIO_TypeDef *port_reg, uint8_t pin, bool lock)
{
    if (lock)
    {
        port_reg->LCKR = ((port_reg->LCKR & ~(1U << GPIO_LCKR_LCKK_Pos)) |
                          (((port_reg->LCKR >> pin) & 1U) ^ 1U) << GPIO_LCKR_LCKK_Pos);
        port_reg->LCKR = ((port_reg->LCKR & ~(1U << GPIO_LCKR_LCKK_Pos)) |
                          (((port_reg->LCKR >> pin) & 1U) ^ 0U) << GPIO_LCKR_LCKK_Pos);
        port_reg->LCKR = ((port_reg->LCKR & ~(1U << GPIO_LCKR_LCKK_Pos)) |
                          (((port_reg->LCKR >> pin) & 1U) ^ 1U) << GPIO_LCKR_LCKK_Pos);
    }
}


void gpio_init(gpio_port_t port, uint8_t pin, gpio_mode_t mode,
               gpio_output_type_t output_type, gpio_speed_t output_speed,
               gpio_pull_t pull, gpio_af_t af, bool lock)
{
    GPIO_TypeDef *port_reg;

    switch (port)
    {
    case GPIO_PORT_A:
        port_reg = GPIOA;
        break;
    case GPIO_PORT_B:
        port_reg = GPIOB;
        break;
    case GPIO_PORT_C:
        port_reg = GPIOC;
        break;
    case GPIO_PORT_D:
        port_reg = GPIOD;
        break;
    case GPIO_PORT_E:
        port_reg = GPIOE;
        break;
    case GPIO_PORT_F:
        port_reg = GPIOF;
        break;
    case GPIO_PORT_G:
        port_reg = GPIOG;
        break;
    }

    gpio_init_mode(port_reg, pin, mode);
    gpio_init_output_type(port_reg, pin, output_type);
    gpio_init_output_speed(port_reg, pin, output_speed);
    gpio_init_pull(port_reg, pin, pull);
    gpio_init_af(port_reg, pin, af);
    gpio_init_lock(port_reg, pin, lock);
}
