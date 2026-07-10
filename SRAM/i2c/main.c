#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/stdio_usb.h"
#include "hardware/i2c.h"
#include "pcf8574_lcd.h"

// Try I2C1/GPIO6-7 first. If the scan finds nothing (including no RTC/EEPROM),
// comment this block out and uncomment the I2C0 block below instead.
#define I2C_PORT   i2c1
#define I2C_SDA_PIN 6 //14
#define I2C_SCL_PIN 7 //working 15 
#include "hardware/adc.h"

#define I2C_BAUD    100000  // 100kHz standard mode, safe for PCF8574

// Quick bus scan so you can confirm the backpack's actual address
// (0x27 vs 0x3F) before trusting the LCD driver
static void i2c_bus_scan(void) {
    printf("Scanning I2C bus...\n");

 for (uint8_t addr = 0x08; addr < 0x78; addr++)
{
    uint8_t dummy;

    printf("Scanning address 0x%02X\n", addr);

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

int main(void) {
    stdio_init_all();

    // Block here until a terminal actually opens the COM port.
    // Without this, printf output sent before you open the terminal
    // is silently dropped and you see nothing.
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    sleep_ms(200); // small settle delay after connect
    
     adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4); 
  
    i2c_init(I2C_PORT, I2C_BAUD);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    printf("I2C init done on SDA=GPIO%d SCL=GPIO%d\n", I2C_SDA_PIN, I2C_SCL_PIN);

    i2c_bus_scan();

    lcd_init(I2C_PORT);
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("HARIKRISHNA");

    printf("Sent 'Hello' to LCD.\n");
     while (0)
    {
        uint16_t raw = adc_read();

        float voltage = raw * 3.3f / 4096.0f;

        float temperature =
            27.0f - (voltage - 0.706f) / 0.001721f;

          printf("Raw = %u\n", raw);
        printf("Voltage = %.3f V\n", voltage);
        printf("Temperature = %.2f C\n\n", temperature);

        sleep_ms(1000);
    }
  while(1){}
}
 

   