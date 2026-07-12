#include <stdio.h>
#include "pico/stdlib.h"

int main()
{
    stdio_init_all();

    gpio_init(11);
    gpio_set_dir(11, GPIO_IN);

    gpio_pull_up(11);

    while (1)
    {
        printf("Pull-up : %d\n", gpio_get(11));
        sleep_ms(1000);
    }
}