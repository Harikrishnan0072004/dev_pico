#include <stdio.h>
#include "pico/stdlib.h"

#define NUM_PINS 8

// Keypad connected to GPIO10-GPIO17
const uint pins[NUM_PINS] = {10,11,12,13,14,15,16,17};

int main()
{
    stdio_init_all();
    sleep_ms(2000);

    printf("\n=== Keypad Scanner ===\n");

    while (true)
    {
        bool key_found = false;

        // Make all pins inputs with pull-ups
        for (int i = 0; i < NUM_PINS; i++)
        {
            gpio_init(pins[i]);
            gpio_set_dir(pins[i], GPIO_IN);
            gpio_pull_up(pins[i]);
        }

        // Drive one pin LOW at a time
        for (int out = 0; out < NUM_PINS && !key_found; out++)
        {
            gpio_set_dir(pins[out], GPIO_OUT);
            gpio_put(pins[out], 0);

            sleep_us(20);

            for (int in = 0; in < NUM_PINS; in++)
            {
                if (in == out)
                    continue;

                if (gpio_get(pins[in]) == 0)
                {
                    printf("Pressed: GP%d <--> GP%d\n",
                           pins[out], pins[in]);

                    // Wait until released
                    while (gpio_get(pins[in]) == 0)
                    {
                        tight_loop_contents();
                    }

                    sleep_ms(50);     // Debounce
                    key_found = true;
                    break;
                }
            }

            gpio_set_dir(pins[out], GPIO_IN);
            gpio_pull_up(pins[out]);
        }

        sleep_ms(5);
    }
}