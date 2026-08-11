#include "lcd_jhd204a.h"

static void pulse_enable(void) {
    gpio_put(LCD_EN, 1);
    sleep_us(1);
    gpio_put(LCD_EN, 0);
    sleep_us(100); // command settle time
}

static void write_nibble(uint8_t nibble) {
    gpio_put(LCD_D4, (nibble >> 0) & 0x01);
    gpio_put(LCD_D5, (nibble >> 1) & 0x01);
    gpio_put(LCD_D6, (nibble >> 2) & 0x01);
    gpio_put(LCD_D7, (nibble >> 3) & 0x01);
    pulse_enable();
}

static void write_byte(uint8_t byte, bool is_data) {
    gpio_put(LCD_RS, is_data ? 1 : 0);
    write_nibble(byte >> 4);   // high nibble first
    write_nibble(byte & 0x0F); // then low nibble
}

void lcd_send_cmd(uint8_t cmd) {
    write_byte(cmd, false);
    if (cmd == 0x01 || cmd == 0x02) sleep_ms(2); // clear/home need longer
}

void lcd_send_data(uint8_t data) {
    write_byte(data, true);
}

void lcd_clear(void) {
    lcd_send_cmd(0x01);
    sleep_ms(2);
}

// 20x4 uses non-contiguous DDRAM row offsets
void lcd_set_cursor(uint8_t col, uint8_t row) {
    static const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    if (row > 3) row = 3;
    lcd_send_cmd(0x80 | (col + row_offsets[row]));
}

void lcd_print(const char *str) {
    while (*str) {
        lcd_send_data((uint8_t)*str++);
    }
}

void lcd_init(void) {
    gpio_init(LCD_RS); gpio_set_dir(LCD_RS, GPIO_OUT);
    gpio_init(LCD_EN); gpio_set_dir(LCD_EN, GPIO_OUT);
    gpio_init(LCD_D4); gpio_set_dir(LCD_D4, GPIO_OUT);
    gpio_init(LCD_D5); gpio_set_dir(LCD_D5, GPIO_OUT);
    gpio_init(LCD_D6); gpio_set_dir(LCD_D6, GPIO_OUT);
    gpio_init(LCD_D7); gpio_set_dir(LCD_D7, GPIO_OUT);

    gpio_put(LCD_RS, 0);
    gpio_put(LCD_EN, 0);

    sleep_ms(50); // power-on wait

    // HD44780 4-bit init sequence (per datasheet, forced 3x + function set)
    write_nibble(0x03);
    sleep_ms(5);
    write_nibble(0x03);
    sleep_us(150);
    write_nibble(0x03);
    sleep_us(150);
    write_nibble(0x02); // switch to 4-bit mode
    sleep_us(150);

    lcd_send_cmd(0x28); // function set: 4-bit, 2-line, 5x8 font
                         // (20x4 controllers use 2-line internal addressing)
    lcd_send_cmd(0x0C); // display ON, cursor off, blink off
    lcd_send_cmd(0x06); // entry mode: increment, no shift
    lcd_send_cmd(0x01); // clear display
    sleep_ms(2);
}