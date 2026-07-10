#ifndef PCF8574_LCD_H
#define PCF8574_LCD_H

#include <stdint.h>
#include "hardware/i2c.h"

// Change to 0x3F if your backpack's A0/A1/A2 jumpers give that address instead
#define LCD_I2C_ADDR   0x27

void lcd_init(i2c_inst_t *i2c);
void lcd_clear(void);
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_print(const char *str);

#endif
