#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <stdio.h>


int main() {
    stdio_init_all();
   while(1){
 gpio_init(5);
gpio_set_dir(5 , GPIO_OUT);
gpio_put(5 , 0 ); 
    sleep_ms(1000);
 gpio_put(5 , 1 ); 
    sleep_ms(1000);
   }
    }
