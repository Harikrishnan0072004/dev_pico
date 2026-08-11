#include "pico/stdlib.h"
#include "lcd_jhd204a.h"

int main() {
    stdio_init_all();
    lcd_init();

    lcd_set_cursor(0, 0);
    lcd_print("JHD204A 20x4 Test");
    lcd_set_cursor(0, 1);
    lcd_print("Row 1");
    lcd_set_cursor(0, 2);
    lcd_print("Row 2");
    lcd_set_cursor(0, 3);
    lcd_print("Row 3");

    while (1) {
        tight_loop_contents();
    }
}