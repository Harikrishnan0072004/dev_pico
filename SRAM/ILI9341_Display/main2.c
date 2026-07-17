#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "font5x7.h"

#define LCD_SPI   spi0
#define PIN_SCK   2
#define PIN_MOSI  3
#define PIN_MISO  4
#define PIN_CS    5
#define PIN_DC    6
#define PIN_RST   7

// Native GRAM: 320 wide x 480 tall (portrait)
#define GRAM_W 320
#define GRAM_H 480

static inline void cmd(uint8_t c) {
    gpio_put(PIN_DC, 0);
    gpio_put(PIN_CS, 0);
    spi_write_blocking(LCD_SPI, &c, 1);
    gpio_put(PIN_CS, 1);
}

static inline void dat(uint8_t d) {
    gpio_put(PIN_DC, 1);
    gpio_put(PIN_CS, 0);
    spi_write_blocking(LCD_SPI, &d, 1);
    gpio_put(PIN_CS, 1);
}

void set_addr_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    cmd(0x2A);
    dat(x0 >> 8); dat(x0 & 0xFF); dat(x1 >> 8); dat(x1 & 0xFF);

    cmd(0x2B);
    dat(y0 >> 8); dat(y0 & 0xFF); dat(y1 >> 8); dat(y1 & 0xFF);

    cmd(0x2C);
}

void fill_screen(uint8_t r, uint8_t g, uint8_t b)
{
    set_addr_window(0, 0, GRAM_W - 1, GRAM_H - 1);

    uint8_t row[GRAM_W * 3];
    for (int i = 0; i < GRAM_W; i++) {
        row[i*3]   = r;
        row[i*3+1] = g;
        row[i*3+2] = b;
    }

    gpio_put(PIN_DC, 1);
    gpio_put(PIN_CS, 0);
    for (int y = 0; y < GRAM_H; y++) {
        spi_write_blocking(LCD_SPI, row, sizeof(row));
    }
    gpio_put(PIN_CS, 1);
}

// x,y in native GRAM coordinates (0..319, 0..479)
void draw_pixel(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b)
{
    if (x >= GRAM_W || y >= GRAM_H) return;

    set_addr_window(x, y, x, y);

    uint8_t px[3] = {r, g, b};
    gpio_put(PIN_DC, 1);
    gpio_put(PIN_CS, 0);
    spi_write_blocking(LCD_SPI, px, 3);
    gpio_put(PIN_CS, 1);
}

// Draws a single 5x7 character, scaled by `size`, at (x,y) = top-left corner
void draw_char(uint16_t x, uint16_t y, char c, uint8_t r, uint8_t g, uint8_t b, uint8_t size)
{
    if (c < 0x20 || c > 0x7A) return;
    const uint8_t *glyph = font5x7[c - 0x20];

    for (int col = 0; col < 5; col++) {
        uint8_t line = glyph[col];
        for (int row = 0; row < 7; row++) {
            if (line & (1 << row)) {
                if (size == 1) {
                    draw_pixel(x + col, y + row, r, g, b);
                } else {
                    // scale up: draw a size x size block per "pixel"
                    for (int dx = 0; dx < size; dx++)
                        for (int dy = 0; dy < size; dy++)
                            draw_pixel(x + col*size + dx, y + row*size + dy, r, g, b);
                }
            }
        }
    }
}

// Draws a null-terminated string starting at (x,y), moving right
void draw_string(uint16_t x, uint16_t y, const char *str, uint8_t r, uint8_t g, uint8_t b, uint8_t size)
{
    uint16_t cursor_x = x;
    while (*str) {
        draw_char(cursor_x, y, *str, r, g, b, size);
        cursor_x += (5 + 1) * size; // 5px glyph + 1px spacing, scaled
        str++;
    }
}

int main()
{
    stdio_init_all();

    spi_init(LCD_SPI, 1 * 1000 * 1000);
    spi_set_format(LCD_SPI, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);

    gpio_init(PIN_CS);  gpio_set_dir(PIN_CS, GPIO_OUT);  gpio_put(PIN_CS, 1);
    gpio_init(PIN_DC);  gpio_set_dir(PIN_DC, GPIO_OUT);  gpio_put(PIN_DC, 1);
    gpio_init(PIN_RST); gpio_set_dir(PIN_RST, GPIO_OUT); gpio_put(PIN_RST, 1);

    gpio_put(PIN_RST, 1); sleep_ms(20);
    gpio_put(PIN_RST, 0); sleep_ms(20);
    gpio_put(PIN_RST, 1); sleep_ms(150);

    cmd(0x01); sleep_ms(150);
    cmd(0x11); sleep_ms(150);
    cmd(0x3A); dat(0x66);   // 18-bit color
    cmd(0x36); dat(0x48);
    cmd(0x29); sleep_ms(50);

    spi_set_baudrate(LCD_SPI, 20 * 1000 * 1000);

    fill_screen(0x00, 0x00, 0x00); // black background

    draw_string(10, 10, "HELLO PICO!", 0xFF, 0xFF, 0xFF, 2);   // white, 2x size
    draw_string(10, 40, "ILI9488 TEST", 0xFF, 0x00, 0x00, 2);  // red, 2x size
    draw_string(10, 70, "abc 123", 0x00, 0xFF, 0x00, 1);       // green, 1x size

    while (1) {
        tight_loop_contents();
    }
}