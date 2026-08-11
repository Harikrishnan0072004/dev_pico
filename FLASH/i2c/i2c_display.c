#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pcf8574_lcd.h"

/*I2C Pins */
#define I2C_PORT    i2c1
#define I2C_SDA_PIN   6 // 14
#define I2C_SCL_PIN   7// working 15 
#define I2C_BAUD    40000  // 100kHz standard mode, safe for PCF8574
static void i2c_bus_scan(void) {
    printf("Scanning I2C bus...\n");
  
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
    sleep_ms(100);
}
    
    printf("Scan done.\n");
}
int main() {
    stdio_init_all();
//   while (!stdio_usb_connected()) {
//          sleep_ms(100);
//      }
    sleep_ms(2000); // small settle delay after connect
    gpio_init(25);
   gpio_set_dir(25 , GPIO_OUT); 
    printf("start>>>>");
    i2c_init(I2C_PORT, I2C_BAUD);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);

  // ---- ADD THESE FOUR LINES TO BOOST GP6 & GP7 ELECTRICS ----
    // gpio_set_drive_strength(I2C_SDA_PIN, GPIO_DRIVE_STRENGTH_12MA); // Force high electrical current drive
    // gpio_set_drive_strength(I2C_SCL_PIN, GPIO_DRIVE_STRENGTH_12MA);
    // gpio_set_slew_rate(I2C_SDA_PIN, GPIO_SLEW_RATE_FAST);           // Sharpens up the waveform transitions
    // gpio_set_slew_rate(I2C_SCL_PIN, GPIO_SLEW_RATE_FAST);
    // -----------------------------------------------------------
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
    printf("I2C init done on SDA=GPIO%d SCL=GPIO%d\n", I2C_SDA_PIN, I2C_SCL_PIN);
    i2c_bus_scan();
    lcd_init(I2C_PORT);
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("HARIKRISHNA");
    printf("Sent 'HARIKRISHA' to LCD.\n");
while(1){
  gpio_put(25 ,1 );
sleep_ms(1000);
  gpio_put(25 ,0 );
sleep_ms(1000);

}
 
}
