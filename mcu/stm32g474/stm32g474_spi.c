#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "stm32g474_spi.h"
#include "stm32g474_types.h"
#include "vendor/Device/ST/STM32G4xx/Include/stm32g474xx.h"


static SPI_TypeDef *spi_get_reg(spi_instance_t instance)
{
    switch (instance)
    {
    case SPI_INSTANCE_1:
        return SPI1;
    case SPI_INSTANCE_2:
        return SPI2;
    case SPI_INSTANCE_3:
        return SPI3;
    case SPI_INSTANCE_4:
        return SPI4;
    }
}


static void spi_config_role(SPI_TypeDef *spi_reg, spi_role_t role)
{
    spi_reg->CR1 =
        ((spi_reg->CR1 & ~SPI_CR1_MSTR) | ((uint32_t)role << SPI_CR1_MSTR_Pos));
}


static void spi_config_mode(SPI_TypeDef *spi_reg, spi_mode_t mode)
{
    spi_reg->CR1 = ((spi_reg->CR1 & ~(3UL << SPI_CR1_CPHA_Pos)) |
                    ((uint32_t)mode << SPI_CR1_CPHA_Pos));
}


static void spi_config_baud(SPI_TypeDef *spi_reg, spi_baud_t baud)
{
    spi_reg->CR1 =
        ((spi_reg->CR1 & ~SPI_CR1_BR_Msk) | ((uint32_t)baud << SPI_CR1_BR_Pos));
}


static void spi_config_bit_order(SPI_TypeDef *spi_reg, spi_bit_order_t bit_order)
{
    spi_reg->CR1 = ((spi_reg->CR1 & ~SPI_CR1_LSBFIRST) |
                    ((uint32_t)bit_order << SPI_CR1_LSBFIRST_Pos));
}


static void spi_config_data_size(SPI_TypeDef *spi_reg, spi_data_size_t data_size)
{
    spi_reg->CR2 =
        ((spi_reg->CR2 & ~SPI_CR2_DS_Msk) | ((uint32_t)data_size) << SPI_CR2_DS_Pos);
}


static void spi_config_nss(SPI_TypeDef *spi_reg, spi_nss_t nss)
{
    spi_reg->CR1 = ((spi_reg->CR1 & ~SPI_CR1_SSM) | ((uint32_t)nss << SPI_CR1_SSM_Pos));
}


static void spi_config_direction(SPI_TypeDef *spi_reg, spi_direction_t direction)
{
    switch (direction)
    {
    case SPI_DIRECTION_FULL_DUPLEX:
    case SPI_DIRECTION_TX_ONLY:
        spi_reg->CR1 = spi_reg->CR1 & ~(SPI_CR1_BIDIMODE | SPI_CR1_RXONLY);
        break;

    case SPI_DIRECTION_RX_ONLY:
        spi_reg->CR1 =
            ((spi_reg->CR1 & ~(SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE | SPI_CR1_RXONLY)) |
             SPI_CR1_RXONLY);
        break;

    case SPI_DIRECTION_1LINE_TX:
        spi_reg->CR1 =
            ((spi_reg->CR1 & ~(SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE | SPI_CR1_RXONLY)) |
             (SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE));
        break;

    case SPI_DIRECTION_1LINE_RX:
        spi_reg->CR1 =
            ((spi_reg->CR1 & ~(SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE | SPI_CR1_RXONLY)) |
             SPI_CR1_BIDIMODE);
        break;
    }
}


bool spi_is_busy(spi_instance_t instance)
{
    SPI_TypeDef *spi_reg = spi_get_reg(instance);
    return (spi_reg->SR & SPI_SR_BSY) != 0U;
}


void spi_config(spi_instance_t instance, spi_role_t role, spi_mode_t mode,
                spi_baud_t baud, spi_bit_order_t bit_order, spi_data_size_t data_size,
                spi_nss_t nss, spi_direction_t direction)
{
    SPI_TypeDef *spi_reg = spi_get_reg(instance);

    while (spi_is_busy(instance))
    {
    }

    spi_reg->CR1 &= ~SPI_CR1_SPE;

    spi_config_role(spi_reg, role);
    spi_config_mode(spi_reg, mode);
    spi_config_baud(spi_reg, baud);
    spi_config_bit_order(spi_reg, bit_order);
    spi_config_data_size(spi_reg, data_size);
    spi_config_nss(spi_reg, nss);
    spi_config_direction(spi_reg, direction);

    spi_reg->CR1 |= SPI_CR1_SPE;
}


void spi_transfer_blocking(spi_instance_t instance, const void *tx, void *rx,
                           size_t frames)
{
    SPI_TypeDef *spi_reg = spi_get_reg(instance);

    const uint16_t *tx_16 = tx;
    uint16_t *rx_16 = rx;
    const uint8_t *tx_8 = tx;
    uint8_t *rx_8 = rx;

    if ((((spi_reg->CR2 & SPI_CR2_DS_Msk) >> SPI_CR2_DS_Pos) > 7))
    {
        for (size_t i = 0; i < frames; ++i)
        {
            while (!(spi_reg->SR & SPI_SR_TXE))
            {
            }
            if (tx == NULL)
            {
                *(volatile uint16_t *)&spi_reg->DR = 0xFFFF;
            }
            else
            {
                *(volatile uint16_t *)&spi_reg->DR = tx_16[i];
            }
            while (!(spi_reg->SR & SPI_SR_RXNE))
            {
            }
            if (rx == NULL)
            {
                (void)*(volatile uint16_t *)&spi_reg->DR;
            }
            else
            {
                *(volatile uint16_t *)&rx_16[i] = spi_reg->DR;
            }
        }
    }
    else
    {
        for (size_t i = 0; i < frames; ++i)
        {
            while (!(spi_reg->SR & SPI_SR_TXE))
            {
            }
            if (tx == NULL)
            {
                *(volatile uint8_t *)&spi_reg->DR = 0xFF;
            }
            else
            {
                *(volatile uint8_t *)&spi_reg->DR = tx_8[i];
            }
            while (!(spi_reg->SR & SPI_SR_RXNE))
            {
            }
            if (rx == NULL)
            {
                (void)*(volatile uint8_t *)&spi_reg->DR;
            }
            else
            {
                rx_8[i] = *(volatile uint8_t *)&spi_reg->DR;
            }
        }
    }

    while (spi_is_busy(instance))
    {
    }
}
