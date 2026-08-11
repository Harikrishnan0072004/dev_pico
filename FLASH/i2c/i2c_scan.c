#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT    i2c1
#define SDA_PIN     6
#define SCL_PIN     7

int main()
{
    stdio_init_all();
   
    // Initialize I2C
    i2c_init(I2C_PORT, 100 * 1000);   // 100 kHz

    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

    // Enable internal pull-ups (external 4.7k pull-ups are recommended)
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    sleep_ms(2000);

    printf("\nRP2040 I2C Scanner\n");

    while (true)
    {
        printf("---------------------------------\n");

        for (uint8_t addr = 0x08; addr < 0x78; addr++)
        {
            uint8_t dummy;

            int ret = i2c_read_blocking(I2C_PORT,
                                        addr,
                                        &dummy,
                                        1,
                                        false);

            if (ret >= 0)
            {
                printf("Found device at 0x%02X\n", addr);
            }
        }

        printf("Scan complete.\n\n");

        sleep_ms(3000);
    }
}