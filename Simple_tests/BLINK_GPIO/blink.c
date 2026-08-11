#include<stdio.h>
#include "pico/stdlib.h"

#define LED 25

int main(){

gpio_init(LED);
gpio_set_dir(LED,GPIO_OUT);
gpio_put(LED,1);
while(1) { 
gpio_put(LED,1);
sleep_ms(1000);
gpio_put(LED,0);
sleep_ms(1000);
 

 }


}
