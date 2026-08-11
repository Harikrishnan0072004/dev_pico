#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT     i2c1
#define SDA_PIN      6
#define SCL_PIN      7
#define TEST_ADDR    0x27   // change to whatever you're testing, e.g. DS1307=0x68, 24LC512=0x50

int main() {
    stdio_init_all();
    sleep_ms(2000);

    gpio_init(25);
    gpio_set_dir(25,GPIO_OUT);
    gpio_put(25,1);


    i2c_init(I2C_PORT, 100* 1000); // 100kHz - keep slow for first trace check
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    printf("Pinging address 0x%02X on i2c%d (SDA=%d, SCL=%d)\n",
           TEST_ADDR, i2c_hw_index(I2C_PORT), SDA_PIN, SCL_PIN);

    while (1) 
{
        uint8_t rxdata;
        int ret = i2c_read_blocking(I2C_PORT, TEST_ADDR, &rxdata, 1, false);

        if (ret < 0) {
            printf("NACK / no response (ret=%d)\n", ret);
        } else {
            printf("ACK! got 0x%02X\n", rxdata);
        }

        sleep_ms(1000); // big gap so each transaction is easy to find in the capture
    }

while(1); 
}
