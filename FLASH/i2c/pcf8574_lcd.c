#include "pcf8574_lcd.h"
#include "pico/stdlib.h"

// ---- PCF8574 bit mapping on the standard LCD backpack ----
// P0 = RS   (0 = command, 1 = data)
// P1 = RW   (we always write, so tie 0)
// P2 = E    (enable strobe, pulse high then low to latch)
// P3 = Backlight (1 = on)
// P4-P7 = D4-D7 (LCD upper nibble bus, 4-bit mode)

#define LCD_RS   (1 << 0)
#define LCD_RW   (1 << 1)
#define LCD_EN   (1 << 2)
#define LCD_BL   (1 << 3)

static i2c_inst_t *lcd_i2c;
static uint8_t backlight_state = LCD_BL;

// Write one raw byte to the PCF8574 (all 8 output pins at once)
static void pcf8574_write(uint8_t data) {
    uint8_t buf = data | backlight_state;
    i2c_write_blocking(lcd_i2c, LCD_I2C_ADDR, &buf, 1, false);
}

// Pulse the Enable line so the HD44780 latches the nibble currently on D4-D7
static void lcd_strobe(uint8_t data) {
    pcf8574_write(data | LCD_EN);
    sleep_us(1);            // E pulse width > 450ns min
    pcf8574_write(data & ~LCD_EN);
    sleep_us(50);           // command settle time
}

// Send one 4-bit nibble (already positioned in bits 4-7) with RS set/clear
static void lcd_write_nibble(uint8_t nibble, uint8_t rs) {
    uint8_t data = (nibble & 0xF0) | rs;
    lcd_strobe(data);
}

// Send a full byte as two nibbles (standard HD44780 4-bit mode transaction)
static void lcd_write_byte(uint8_t val, uint8_t rs) {
    lcd_write_nibble(val & 0xF0, rs);
    lcd_write_nibble((val << 4) & 0xF0, rs);
}

static void lcd_cmd(uint8_t cmd) {
    lcd_write_byte(cmd, 0);
    if (cmd == 0x01 || cmd == 0x02) {
        sleep_ms(2); // clear/home need extra time
    }
}

static void lcd_data(uint8_t data) {
    lcd_write_byte(data, LCD_RS);
}

void lcd_clear(void) {
    lcd_cmd(0x01);
}

void lcd_set_cursor(uint8_t col, uint8_t row) {
    static const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    lcd_cmd(0x80 | (col + row_offsets[row]));
}

void lcd_print(const char *str) {
    while (*str) {
        lcd_data((uint8_t)(*str++));
    }
}

void lcd_init(i2c_inst_t *i2c) {
    lcd_i2c = i2c;

    sleep_ms(50); // HD44780 needs >40ms after power-up before first command

    // Force 8-bit -> 4-bit init sequence (standard HD44780 datasheet init).
    // These three writes are sent as upper-nibble-only because the display
    // powers up in 8-bit mode; this is what actually switches it to 4-bit.
    lcd_write_nibble(0x30, 0);
    sleep_ms(5);
    lcd_write_nibble(0x30, 0);
    sleep_us(150);
    lcd_write_nibble(0x30, 0);
    sleep_us(150);
    lcd_write_nibble(0x20, 0); // now switch to 4-bit mode
    sleep_us(150);

    lcd_cmd(0x28); // function set: 4-bit, 2 line, 5x8 font
    lcd_cmd(0x08); // display off
    lcd_cmd(0x01); // clear display
    sleep_ms(2);
    lcd_cmd(0x06); // entry mode: increment cursor, no shift
    lcd_cmd(0x0C); // display on, cursor off, blink off
}
