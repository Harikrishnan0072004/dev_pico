

#include "pico/stdlib.h"

#define LCD_RS 19
#define LCD_EN 20
#define LCD_D4 21
#define LCD_D5 22
#define LCD_D6 23
#define LCD_D7 24

void lcd_init(void);
void lcd_clear(void);
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_print(const char *str);
void lcd_send_cmd(uint8_t cmd);
void lcd_send_data(uint8_t data);

#endif