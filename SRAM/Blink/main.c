#include <stdio.h>
#include "pico/stdlib.h"
#include  "hardware/gpio.h"

#define LED 6
#define LED1 7
#define LED2 25
 int main() {
     stdio_init_all();
        gpio_init(LED);
        gpio_init(LED1);
        gpio_init(LED2);
        gpio_set_dir(LED , GPIO_OUT);
  
     gpio_set_dir(LED1 , GPIO_OUT);
 
  
     gpio_set_dir(LED2 , GPIO_OUT);
while(1){    
 gpio_put(LED  ,1  ); 
 gpio_put(LED1 ,1 );
 gpio_put(LED2 ,1 );
 sleep_ms(1000);
 gpio_put(LED , 0 );
 gpio_put(LED1 ,0 );
 gpio_put(LED2 ,0 );
sleep_ms(1000);

 }
 }