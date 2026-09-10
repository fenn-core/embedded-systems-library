#include <stdbool.h>
#include <stdint.h>
#include "stm32g474_gpio.h"
#include "vendor/Device/ST/STM32G4xx/Include/stm32g474xx.h"


static GPIO_TypeDef *gpio_port_selector(gpio_port_t port)
{
    switch (port)
    {
    case GPIO_PORT_A:
        return GPIOA;
    case GPIO_PORT_B:
        return GPIOB;
    case GPIO_PORT_C:
        return GPIOC;
    case GPIO_PORT_D:
        return GPIOD;
    case GPIO_PORT_E:
        return GPIOE;
    case GPIO_PORT_F:
        return GPIOF;
    case GPIO_PORT_G:
        return GPIOG;
    }
}


static void gpio_config_mode(GPIO_TypeDef *port_reg, uint8_t pin, gpio_mode_t mode)
{
    port_reg->MODER =
        ((port_reg->MODER & ~(3UL << (pin * 2))) | ((uint32_t)mode << (pin * 2)));
}


static void gpio_config_output_type(GPIO_TypeDef *port_reg, uint8_t pin,
                                    gpio_output_type_t output_type)
{
    port_reg->OTYPER =
        (port_reg->OTYPER & (~(1UL << pin)) | ((uint32_t)output_type << pin));
}


static void gpio_config_output_speed(GPIO_TypeDef *port_reg, uint8_t pin,
                                     gpio_speed_t speed)
{
    port_reg->OSPEEDR =
        ((port_reg->OSPEEDR & ~(3UL << (pin * 2))) | ((uint32_t)speed << (pin * 2)));
}


static void gpio_config_pull(GPIO_TypeDef *port_reg, uint8_t pin, gpio_pull_t pull)
{
    port_reg->PUPDR =
        ((port_reg->PUPDR & ~(3UL << (pin * 2))) | ((uint32_t)pull << (pin * 2)));
}


static void gpio_config_af(GPIO_TypeDef *port_reg, uint8_t pin, gpio_af_t af)
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


void gpio_pin_config(gpio_port_t port, uint8_t pin, gpio_mode_t mode,
                     gpio_output_type_t output_type, gpio_speed_t output_speed,
                     gpio_pull_t pull, gpio_af_t af)
{
    GPIO_TypeDef *port_reg = gpio_port_selector(port);

    gpio_config_mode(port_reg, pin, mode);
    gpio_config_output_type(port_reg, pin, output_type);
    gpio_config_output_speed(port_reg, pin, output_speed);
    gpio_config_pull(port_reg, pin, pull);
    gpio_config_af(port_reg, pin, af);
}


bool gpio_lock(gpio_port_t port, uint16_t pin_mask)
{
    GPIO_TypeDef *port_reg = gpio_port_selector(port);

    if (pin_mask & GPIO_LCKR_LCKK_Msk)
    {
        return false;
    }
    port_reg->LCKR = GPIO_LCKR_LCKK_Msk | (uint32_t)pin_mask;
    port_reg->LCKR = (uint32_t)pin_mask;
    port_reg->LCKR = GPIO_LCKR_LCKK_Msk | (uint32_t)pin_mask;

    return (port_reg->LCKR & GPIO_LCKR_LCKK_Msk) != 0U;
}


bool gpio_read(gpio_port_t port, uint8_t pin)
{
    GPIO_TypeDef *port_reg = gpio_port_selector(port);
    return ((port_reg->IDR >> pin) & 1UL) != 0;
}


bool gpio_get_output_latch(gpio_port_t port, uint8_t pin)
{
    GPIO_TypeDef *port_reg = gpio_port_selector(port);
    return ((port_reg->ODR >> pin) & 1UL) != 0;
}


void gpio_write(gpio_port_t port, uint8_t pin, bool state)
{
    GPIO_TypeDef *port_reg = gpio_port_selector(port);
    if (state)
    {
        port_reg->BSRR = 1UL << pin;
    }
    else
    {
        port_reg->BSRR = 1UL << (pin + 16U);
    }

}
