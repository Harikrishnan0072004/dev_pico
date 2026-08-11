#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c1
#define SDA_PIN  6
#define SCL_PIN  7

int main() {
    stdio_init_all();
    sleep_ms(4000); // give USB-CDC time to enumerate

    i2c_init(I2C_PORT, 10 * 1000); // 100kHz
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    printf("\nI2C Bus Scan on i2c%d (SDA=%d, SCL=%d)\n",
           i2c_hw_index(I2C_PORT), SDA_PIN, SCL_PIN);
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

    for (int addr = 0; addr < 128; addr++) {
        if (addr % 16 == 0) printf("%02x ", addr);

        // Skip reserved address ranges (0x00-0x07, 0x78-0x7F)
        int ret;
        uint8_t rxdata;
        if (addr < 0x08 || addr > 0x77) {
            ret = PICO_ERROR_GENERIC;
        } else {
            ret = i2c_read_blocking(I2C_PORT, addr, &rxdata, 1, false);
        }

        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
    }

    printf("\nScan complete.\n");
    while (1) tight_loop_contents();
}