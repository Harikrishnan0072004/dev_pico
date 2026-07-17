
#include<stdio.h>
#include"pico/stdlib.h"
#include "pico/stdio_usb.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "pcf8574_lcd.h"
#include "hardware/spi.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
enum {
	I2C_LCD = 1,
	I2C_RTC,
	GPIO_LED,
	SPI_MC,
	UART_RWn,
	USB_RWn
};
/*UART - Pins */
#define UART0_ID uart0
#define  UART0_TX 0 
#define   UART0_RX 1
#define  UART1_TX 8
#define  UART1_RX 9
#define UART1_ID uart1
#define BANDRATE 115200
#define LED 25


/*GLOBAL PINS */
#define LED1 10
#define LED2 11
#define LED3 12
#define LED4 13
#define LED5 14
#define LED6 15
#define LED7 16
#define LED8 17
#define LED9 25
/* SPI - Pins  */

#define SPI_PORT spi0
#define PIN_SCK   2
#define PIN_MOSI  3
#define PIN_MISO  4
#define PIN_CS    5

/*I2C Pins */
#define I2C_PORT    i2c1
#define I2C_SDA_PIN   14  // 14
#define I2C_SCL_PIN   15  // working 15 
#define I2C_BAUD    100000  // 100kHz standard mode, safe for PCF8574

 /*KEY_PAD  */

// const uint ROW_PINS[4] = {10, 11, 12, 13};
// const uint COL_PINS[4] = {14, 15, 16, 17};
#define EXIT_KEY 'E'
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
/*SPI -TFT DISPLAY */

#include "font5x7.h"

#define LCD_SPI   spi0
#define PIN_SCK   2
#define PIN_MOSI  3
#define PIN_MISO  4
#define PIN_CS    5
#define PIN_DC    6
#define PIN_RST   7

// Native GRAM: 320 wide x 480 tall (portrait)
#define GRAM_W 320
#define GRAM_H 480

/*Fordware Declaration */
void I2C_LCD1();
static void i2c_bus_scan(void);
void gpio_pins(void);
void SPI_MC_TEST(void);
void UART_RW(void);
char keypad_scan(void);
char keypad_init(void);
void KEY_PAD(void);
void  USB_RW(void);
void SPI_DISPLAY(void);
void draw_string(uint16_t x, uint16_t y, const char *str, uint8_t r, uint8_t g, uint8_t b, uint8_t size);
void draw_char(uint16_t x, uint16_t y, char c, uint8_t r, uint8_t g, uint8_t b, uint8_t size);
void draw_pixel(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b);
void fill_screen(uint8_t r, uint8_t g, uint8_t b);
void set_addr_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
static inline void cmd(uint8_t c);
static inline void dat(uint8_t d);
int main() {
    stdio_init_all();
	
    gpio_init(25);
    gpio_set_dir(25 , GPIO_OUT ); 
   
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
	printf("Peripharal Scanner & Tester\n");
	while(1) {
         gpio_put(25 ,1 );
		printf("\n1. I2C  - LCD\n"
		"2. I2C  - RTC\n"
		"3. GPIO - LED\n"
		"4. SPI  - DISPLAY \n"
		"5. UART - Read/Write\n"
		"6. USB  - Read/Write Pen-Drive\n"
        "7. KEY  - keypad ");

        int choice = 0;
		printf("\nPheripharal to be tested\n");
	    int ret = 0 ; 
        
        char ch = 0 ;
           ch = getchar();
        printf("\nReceived '%c' (%d)\n", ch, ch);
		switch(ch)
		{
			case '1':
				printf("I2C_LCD Test Start\n");
                I2C_LCD1();
			break;

			case  '2':
			
			  printf("RTC- unavailable \n");
			break;

			case  '3':
                printf("GPIO_LED Test\n");
                gpio_pins(); 
			break;

			case  '4':
				printf("SPI_DISPLAYTest\n");
                  SPI_DISPLAY();
               // SPI_MC_TEST();
                
			break;

			case  '5':
				printf("UART_RW Test\n");
                 UART_RW();
			break;

			case  '6':
				printf("USB_RW Test - unavailable \n");
                USB_RW();
			break;
            case  '7': 
               printf("KEY_PAD Test Start \n");
                KEY_PAD();
            break;

			default:
				printf("Invalid pheripherals\n");
		}
     
	}
return 0;
}
void I2C_LCD1(){
    
    stdio_init_all();
    sleep_ms(200); // small settle delay after connect
    i2c_init(I2C_PORT, I2C_BAUD);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
    printf("I2C init done on SDA=GPIO%d SCL=GPIO%d\n", I2C_SDA_PIN, I2C_SCL_PIN);
    i2c_bus_scan();
    lcd_init(I2C_PORT);
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_print("HARIKRISHNA");
    printf("Sent 'HARIKRISHA' to LCD.\n");
 
}
static void i2c_bus_scan(void) {
    printf("Scanning I2C bus...\n");
  
 for (uint8_t addr = 0x08; addr < 0x78; addr++)
{
    uint8_t dummy;
    int ret = i2c_read_blocking(I2C_PORT,
                                addr,
                                &dummy,
                                1,
                                false);

    if (ret >= 0)
    {
        printf("Found device at 0x%02X\n", addr);
    }
    sleep_ms(100);
}
    
    printf("Scan done.\n");
}
void gpio_pins(void) {
    int leds[] = {LED1, LED2, LED3, LED4, LED5, LED6, LED7, LED8, LED9};
    int n = sizeof(leds) / sizeof(leds[0]);

    for (int i = 0; i < n; i++) {
        gpio_init(leds[i]);
        gpio_set_dir(leds[i], GPIO_OUT);
    }

    printf("Blinking LEDs 5 times (press any key to stop early is not checked here)\n");
    for (int cycle = 0; cycle < 5; cycle++) {
        for (int i = 0; i < n; i++) gpio_put(leds[i], 1);
        sleep_ms(1000);
        for (int i = 0; i < n; i++) gpio_put(leds[i], 0);
        sleep_ms(1000);
          int ch = getchar_timeout_us(0);
          if (ch == 'q' || ch == 'Q')
           {
            printf("Returning to main menu...\n");
            return;
}
    }
    printf("GPIO_LED test done.\n");



}
	
void SPI_MC_TEST(void)

{
    // Initialize SPI1 at 1 MHz
    spi_init(SPI_PORT, 1000 * 1000);

    // Configure SPI pins
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS,   GPIO_FUNC_SPI);

    sleep_ms(2000);

    uint8_t tx = 0x55;
    uint8_t rx = 0;

    while (1)
    {
        rx = 0;

        spi_write_read_blocking(SPI_PORT, &tx, &rx, 1);

        printf("TX = 0x%02X   RX = 0x%02X   ", tx, rx);
        int ch = getchar_timeout_us(0);

        if (tx == rx)
      
            printf("PASS\r\n"); 
        else
            printf("FAIL\r\n");

        tx++;
    //quite the while 
        for (int i = 0; i < 10; i++) {
            int ch2 = getchar_timeout_us(0);
            if (ch2 == 'q' || ch2 == 'Q') {
                printf("Returning to main menu...\n");
                return;
            }
            sleep_ms(100);
        }  
      } 
    }


void UART_RW(void){

        gpio_init(LED);
        gpio_set_dir(LED , GPIO_OUT);
        uart_init(UART0_ID, BANDRATE);
        gpio_set_function(UART0_TX , GPIO_FUNC_UART);
        gpio_set_function(UART0_RX , GPIO_FUNC_UART);
        
        uart_init(UART1_ID, BANDRATE);
        gpio_set_function(UART1_TX , GPIO_FUNC_UART);
        gpio_set_function(UART1_RX , GPIO_FUNC_UART);
        gpio_put(LED ,1);
        uart_puts(UART0_ID, " HELLOW  FROM UART0 ..\n");
        uart_puts(UART1_ID, " HELLOW  FROM UART1 ..\n");
        while(1){  
       
        printf("check usb cdc work \n");
        
        //check UART0 RX 
        if(uart_is_readable(UART0_ID) ){   //rx and tx loop backed its work , okey
            char c0 = uart_getc(UART0_ID);   //if uart0 get data ,then print happen otherwise else
            printf("uart0 RESIVED %c\n",c0);
        }
        else{
            printf(" uart0 get nothing \n");
        }
        //check uart1 RX 
        if(uart_is_readable(UART1_ID)){  
            char c1 = uart_getc(UART1_ID);  
        printf(" uart1 resived %c \n", c1);
        }
        else { 
            printf("uart1 get nothing \n");
        }
        //quite the while 
        for (int i = 0; i < 10; i++) {
            int ch2 = getchar_timeout_us(0);
            if (ch2 == 'q' || ch2 == 'Q') {
                printf("Returning to main menu...\n");
                return;
            }
            sleep_ms(100);
        // sleep_ms(1000); 
        }
 }
}
void KEY_PAD(void){
 {
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
       // sleep_ms(10);
           //quite the while 
   if (key == EXIT_KEY) {
                printf("Exit key pressed. Returning to main menu...\n");
                return;
            }
    }
    // return 0;
}
}

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
    return 0;
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
                        //quite the while 
                    while (gpio_get(ROW_PINS[r]) == 0) {   // wait for release
                        sleep_ms(5);
                    }
                    return k;
                }
            }
        }
    }
    return 0;
}

/* SPI - DISPLAY */
   void SPI_DISPLAY(void){

    stdio_init_all();

    spi_init(LCD_SPI, 1 * 1000 * 1000);
    spi_set_format(LCD_SPI, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);

    gpio_init(PIN_CS);  gpio_set_dir(PIN_CS, GPIO_OUT);  gpio_put(PIN_CS, 1);
    gpio_init(PIN_DC);  gpio_set_dir(PIN_DC, GPIO_OUT);  gpio_put(PIN_DC, 1);
    gpio_init(PIN_RST); gpio_set_dir(PIN_RST, GPIO_OUT); gpio_put(PIN_RST, 1);

    gpio_put(PIN_RST, 1); sleep_ms(20);
    gpio_put(PIN_RST, 0); sleep_ms(20);
    gpio_put(PIN_RST, 1); sleep_ms(150);

    cmd(0x01); sleep_ms(150);
    cmd(0x11); sleep_ms(150);
    cmd(0x3A); dat(0x66);   // 18-bit color
    cmd(0x36); dat(0x48);
    cmd(0x29); sleep_ms(50);

    spi_set_baudrate(LCD_SPI, 20 * 1000 * 1000);

    fill_screen(0x00, 0x00, 0x00); // black background

    draw_string(10, 10, "HELLO PICO!", 0xFF, 0xFF, 0xFF, 2);   // white, 2x size
    draw_string(10, 40, "ILI9488 TEST", 0xFF, 0x00, 0x00, 2);  // red, 2x size
    draw_string(10, 70, "abc 123", 0x00, 0xFF, 0x00, 1);       // green, 1x size

    while (1) {
        tight_loop_contents();
    }


}


static inline void cmd(uint8_t c) {
    gpio_put(PIN_DC, 0);
    gpio_put(PIN_CS, 0);
    spi_write_blocking(LCD_SPI, &c, 1);
    gpio_put(PIN_CS, 1);
}

static inline void dat(uint8_t d) {
    gpio_put(PIN_DC, 1);
    gpio_put(PIN_CS, 0);
    spi_write_blocking(LCD_SPI, &d, 1);
    gpio_put(PIN_CS, 1);
}

void set_addr_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    cmd(0x2A);
    dat(x0 >> 8); dat(x0 & 0xFF); dat(x1 >> 8); dat(x1 & 0xFF);

    cmd(0x2B);
    dat(y0 >> 8); dat(y0 & 0xFF); dat(y1 >> 8); dat(y1 & 0xFF);

    cmd(0x2C);
}

void fill_screen(uint8_t r, uint8_t g, uint8_t b)
{
    set_addr_window(0, 0, GRAM_W - 1, GRAM_H - 1);

    uint8_t row[GRAM_W * 3];
    for (int i = 0; i < GRAM_W; i++) {
        row[i*3]   = r;
        row[i*3+1] = g;
        row[i*3+2] = b;
    }

    gpio_put(PIN_DC, 1);
    gpio_put(PIN_CS, 0);
    for (int y = 0; y < GRAM_H; y++) {
        spi_write_blocking(LCD_SPI, row, sizeof(row));
    }
    gpio_put(PIN_CS, 1);
}

// x,y in native GRAM coordinates (0..319, 0..479)
void draw_pixel(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b)
{
    if (x >= GRAM_W || y >= GRAM_H) return;

    set_addr_window(x, y, x, y);

    uint8_t px[3] = {r, g, b};
    gpio_put(PIN_DC, 1);
    gpio_put(PIN_CS, 0);
    spi_write_blocking(LCD_SPI, px, 3);
    gpio_put(PIN_CS, 1);
}

// Draws a single 5x7 character, scaled by `size`, at (x,y) = top-left corner
void draw_char(uint16_t x, uint16_t y, char c, uint8_t r, uint8_t g, uint8_t b, uint8_t size)
{
    if (c < 0x20 || c > 0x7A) return;
    const uint8_t *glyph = font5x7[c - 0x20];

    for (int col = 0; col < 5; col++) {
        uint8_t line = glyph[col];
        for (int row = 0; row < 7; row++) {
            if (line & (1 << row)) {
                if (size == 1) {
                    draw_pixel(x + col, y + row, r, g, b);
                } else {
                    // scale up: draw a size x size block per "pixel"
                    for (int dx = 0; dx < size; dx++)
                        for (int dy = 0; dy < size; dy++)
                            draw_pixel(x + col*size + dx, y + row*size + dy, r, g, b);
                }
            }
        }
    }
}

// Draws a null-terminated string starting at (x,y), moving right
void draw_string(uint16_t x, uint16_t y, const char *str, uint8_t r, uint8_t g, uint8_t b, uint8_t size)
{
    uint16_t cursor_x = x;
    while (*str) {
        draw_char(cursor_x, y, *str, r, g, b, size);
        cursor_x += (5 + 1) * size; // 5px glyph + 1px spacing, scaled
        str++;
    }
}




void USB_RW(void){
 printf(" HI from UART_RW \n");
}
