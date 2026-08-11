#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <stdio.h>

//const uint ROW_PINS[4] = {0, 0, 14, 15};
//const uint COL_PINS[4] = {10, 11, 12, 13};

//working line 
 const uint ROW_PINS[4] = {15, 13, 11, 10};
 const uint COL_PINS[4] = {16, 17, 14, 12};


const char keymap[4][4] = {
    // row 0:
    { '1', '2', '3', '4', },   

    // row 1: 
    { '5', '6', '7', '8' },    

    // row 2: 
    { '9', '0', 'E', '<' },

    // row 3:
    { 'D', '>', 'K', 'U' }
};


char keypad_init(void) {
    // Initialize rows as inputs with pull-up
    for (int r = 0; r < 4; r++) {
        gpio_init(ROW_PINS[r]);
        gpio_set_dir(ROW_PINS[r], GPIO_IN);
        // gpio_pull_up(ROW_PINS[r]);  // rows idle HIGH
    }

    // Initialize columns as inputs (idle) for now
    for (int c = 0; c < 4; c++) {
        gpio_init(COL_PINS[c]);
        gpio_set_dir(COL_PINS[c], GPIO_IN);  // will switch to OUTPUT during scan
    }
}
char keypad_scan(void) { 

   for (int c = 0; c < 4; c++) {

        // 1) Set all columns to input (high impedance)
        for (int cc = 0; cc < 4; cc++) {
             gpio_init(COL_PINS[cc]);
            gpio_set_dir(COL_PINS[cc], GPIO_IN);
        }

        // 2) Make this column OUTPUT and drive it LOW
        gpio_set_dir(COL_PINS[c], GPIO_OUT);
        gpio_put(COL_PINS[c], 0);

        // 3) Read all rows
        for (int r = 0; r < 4; r++) {
            if (gpio_get(ROW_PINS[r]) == 0) {
                sleep_ms(20);
                if (gpio_get(ROW_PINS[r]) == 0) {
                    while (gpio_get(ROW_PINS[r]) == 0) {
                        sleep_ms(5);
                    }

                    char k = keymap[r][c];

                    // print raw row/col AND mapped key
                    printf("raw hit: row=%d col=%d  key='%c'\n", r, c, k);

                    return k;
                }
            }
        }
    }
    return 0;
}
int main() {

    stdio_init_all();
    // onboard LED (GPIO 25) just as a power/check indicator
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 1);
    keypad_init();
    printf("4x4 keypad test started.\n");
   
     while (true) {     
        char key = keypad_scan() ;
        if (key != 0) {
            printf("KEY PRESSED: %c\n", key);
        }
        sleep_ms(10);
    }
    return 0;
}
