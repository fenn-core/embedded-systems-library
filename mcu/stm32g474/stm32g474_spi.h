#ifndef INC_STM32G474_SPI_H_
#define INC_STM32G474_SPI_H_

#include <stdbool.h>
#include "stm32g474_types.h"


typedef enum
{
    SPI_ROLE_SLAVE = 0,
    SPI_ROLE_MASTER = 1,
} spi_role_t;


typedef enum
{
    SPI_MODE_0 = 0,
    SPI_MODE_1 = 1,
    SPI_MODE_2 = 2,
    SPI_MODE_3 = 3,
} spi_mode_t;


typedef enum
{
    SPI_BAUD_DIV2 = 0,
    SPI_BAUD_DIV4 = 1,
    SPI_BAUD_DIV8 = 2,
    SPI_BAUD_DIV16 = 3,
    SPI_BAUD_DIV32 = 4,
    SPI_BAUD_DIV64 = 5,
    SPI_BAUD_DIV128 = 6,
    SPI_BAUD_DIV256 = 7,
} spi_baud_t;


typedef enum
{
    SPI_MSB_FIRST = 0,
    SPI_LSB_FIRST = 1,
} spi_bit_order_t;


typedef enum
{
    SPI_DATA_SIZE_4_BIT = 3,
    SPI_DATA_SIZE_5_BIT = 4,
    SPI_DATA_SIZE_6_BIT = 5,
    SPI_DATA_SIZE_7_BIT = 6,
    SPI_DATA_SIZE_8_BIT = 7,
    SPI_DATA_SIZE_9_BIT = 8,
    SPI_DATA_SIZE_10_BIT = 9,
    SPI_DATA_SIZE_11_BIT = 10,
    SPI_DATA_SIZE_12_BIT = 11,
    SPI_DATA_SIZE_13_BIT = 12,
    SPI_DATA_SIZE_14_BIT = 13,
    SPI_DATA_SIZE_15_BIT = 14,
    SPI_DATA_SIZE_16_BIT = 15,
} spi_data_size_t;


typedef enum
{
    SPI_NSS_HARDWARE = 0,
    SPI_NSS_SOFTWARE = 1,
} spi_nss_t;


typedef enum
{
    SPI_DIRECTION_FULL_DUPLEX = 0,
    SPI_DIRECTION_TX_ONLY = 1,
    SPI_DIRECTION_RX_ONLY = 2,
    SPI_DIRECTION_1LINE_TX = 3,
    SPI_DIRECTION_1LINE_RX = 4,
} spi_direction_t;


void spi_config(spi_instance_t instance, spi_role_t role, spi_mode_t mode,
                spi_baud_t baud, spi_bit_order_t bit_order, spi_data_size_t data_size,
                spi_nss_t nss, spi_direction_t direction);


bool spi_is_busy(spi_instance_t instance);


void spi_transfer_blocking(spi_instance_t instance, const void *tx, void *rx,
                           size_t frames);


#endif