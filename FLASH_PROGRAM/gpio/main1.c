#include <stdio.h>
#include "pico/stdlib.h"

#define INPUT_PIN 15  // GPIO connected to transistor collector

int main() {
    stdio_init_all();

    // Configure pin as input only
    gpio_init(INPUT_PIN);
    gpio_set_dir(INPUT_PIN, GPIO_IN);

    while (true) {
        bool state = gpio_get(INPUT_PIN);
        if (state) {
            printf("Pin is HIGH (switch open)\n"); //pullup
        } else {
            printf("Pin is LOW (switch pressed)\n");
        }
        sleep_ms(200);
    }
}
