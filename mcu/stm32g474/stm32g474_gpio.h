#ifndef INC_STM32G474_H_
#define INC_STM32G474_H_

#include <stdbool.h>
#include <stdint.h>


typedef enum
{
    GPIO_PORT_A = 0,
    GPIO_PORT_B = 1,
    GPIO_PORT_C = 2,
    GPIO_PORT_D = 3,
    GPIO_PORT_E = 4,
    GPIO_PORT_F = 5,
    GPIO_PORT_G = 6,
} gpio_port_t;


typedef enum
{
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT = 1,
    GPIO_MODE_ALTERNATE = 2,
    GPIO_MODE_ANALOG = 3,
} gpio_mode_t;


typedef enum
{
    GPIO_OUTPUT_PUSH_PULL = 0,
    GPIO_OUTPUT_OPEN_DRAIN = 1,
} gpio_output_type_t;


typedef enum
{
    GPIO_LOW_SPEED = 0,
    GPIO_MEDIUM_SPEED = 1,
    GPIO_HIGH_SPEED = 2,
    GPIO_VERY_HIGH_SPEED = 3,
} gpio_speed_t;


typedef enum
{
    GPIO_PULL_NONE = 0,
    GPIO_PULL_UP = 1,
    GPIO_PULL_DOWN = 2,
} gpio_pull_t;


typedef enum
{
    GPIO_AF0 = 0,
    GPIO_AF1 = 1,
    GPIO_AF2 = 2,
    GPIO_AF3 = 3,
    GPIO_AF4 = 4,
    GPIO_AF5 = 5,
    GPIO_AF6 = 6,
    GPIO_AF7 = 7,
    GPIO_AF8 = 8,
    GPIO_AF9 = 9,
    GPIO_AF10 = 10,
    GPIO_AF11 = 11,
    GPIO_AF12 = 12,
    GPIO_AF13 = 13,
    GPIO_AF14 = 14,
    GPIO_AF15 = 15,
} gpio_af_t;


void gpio_pin_config(gpio_port_t port, uint8_t pin, gpio_mode_t mode,
                     gpio_output_type_t output_type, gpio_speed_t output_speed,
                     gpio_pull_t pull, gpio_af_t af);


bool gpio_lock(gpio_port_t port, uint16_t pin_mask);


bool gpio_read(gpio_port_t port, uint8_t pin);


bool gpio_get_output_latch(gpio_port_t port, uint8_t pin);


void gpio_write(gpio_port_t port, uint8_t pin, bool state);


#endif
