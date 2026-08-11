#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define UART0_ID uart0
#define  UART0_TX 0 
#define   UART0_RX 1
#define  UART1_TX 8
#define  UART1_RX 9
#define UART1_ID uart1
#define BANDRATE 115200
#define LED 25
int main(){
    stdio_init_all();
sleep_ms(1000);
     gpio_init(LED);
     gpio_set_dir(LED , GPIO_OUT);
     uart_init(UART0_ID, BANDRATE);
     gpio_set_function(UART0_TX , GPIO_FUNC_UART);
     gpio_set_function(UART0_RX , GPIO_FUNC_UART);
     
     uart_init(UART1_ID, BANDRATE);
     gpio_set_function(UART1_TX , GPIO_FUNC_UART);
     gpio_set_function(UART1_RX , GPIO_FUNC_UART);
while(1){
  gpio_put(LED ,1);
   sleep_ms(1000);
  // uart_putc_raw(UART_ID , 'A');
   //uart_putc_raw(UART_ID , '\n'); 
// uart_puts(UART0_ID, " HELLOW  FROM UART0 ..\n");
 uart_puts(UART1_ID, " HELLOW  FROM UART1 ..\n");
  printf("check usb cdc work \n");
   gpio_put(LED , 0);
  sleep_ms(1000); 
    
 }
     

 }