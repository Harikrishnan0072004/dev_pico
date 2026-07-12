#include<stdio.h>
#include"pico/stdlib.h"

int main(){
stdio_init_all();
// gpio_init(16);
// gpio_set_dir(16, GPIO_IN);
// gpio_pull_up(16);
// gpio_init(17);
// gpio_set_dir(17, GPIO_IN);
// //gpio_pull_up(17);
// gpio_pull_down(17);
// gpio_init(25);
// gpio_set_dir(25, GPIO_OUT);
// gpio_put(25 ,1);
gpio_init(16);
gpio_set_dir(16, GPIO_IN);

while (1) {
    printf("%d\n", gpio_get(16));
    sleep_ms(500);
}

// while (1)
// {
//     printf("gpio16 :pullup %d\n", gpio_get(16));

//     printf("gpio17 :pulldown %d\n", gpio_get(17));
//    sleep_ms(1000);
// }
}