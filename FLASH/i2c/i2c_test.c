#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c1
#define SDA_PIN 14 //6
#define SCL_PIN  15
 
#define ADDR 0x27

int main()
{
    stdio_init_all();

 sleep_ms(3000);
       // while (!stdio_usb_connected()) {
         //sleep_ms(100);
    // }
    // Initialize I2C at 100 kHz
    i2c_init(I2C_PORT, 100 * 1000);
  gpio_init(25);
  gpio_set_dir(25 ,GPIO_OUT);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

    // gpio_pull_up(SDA_PIN);
    // gpio_pull_up(SCL_PIN);

   printf("I2C Test Started\n");

    while (1)
    { 
 
 
 
 
        // Write register address 0x00
        uint8_t buf[1] = {0x01};

        int ret = i2c_write_blocking(I2C_PORT,
                                     ADDR,
                                     buf,
                                     1,
                                     false);
 
 
        printf("Write Result = %d\n", ret);
 gpio_put(25 ,1 );
sleep_ms(200);
  gpio_put(25 ,0 );

       
    }
while (1)
{
    tight_loop_contents();
}
}