#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define SPI_PORT spi0

#define PIN_SCK   2
#define PIN_MOSI  3
#define PIN_MISO  4
#define PIN_CS    5

int main()
{
    stdio_init_all();

    // Initialize SPI1 at 1 MHz
    spi_init(SPI_PORT, 1000 * 1000);

    // Configure SPI pins
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS,   GPIO_FUNC_SPI);

    sleep_ms(2000);

    uint8_t tx = 0x55;
    uint8_t rx = 0;

    while (1)
    {
        rx = 0;

        spi_write_read_blocking(SPI_PORT, &tx, &rx, 1);

        printf("TX = 0x%02X   RX = 0x%02X   ", tx, rx);

        if (tx == rx)
            printf("PASS\r\n");
        else
            printf("FAIL\r\n");

        tx++;

        sleep_ms(1000);
    }
}