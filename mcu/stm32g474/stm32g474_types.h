#ifndef INC_STM32G474_TYPES_H_
#define INC_STM32G474_TYPES_H_


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
    SPI_INSTANCE_1 = 1,
    SPI_INSTANCE_2 = 2,
    SPI_INSTANCE_3 = 3,
    SPI_INSTANCE_4 = 4,
} spi_instance_t;



#endif