#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "test.pio.h"

#define LED 25

int main() {
    stdio_init_all();

    PIO pio = pio0;   // use PIO block 0
    uint sm = 0;      // use state machine 0

    // test.pio has ".program test"
    // so generated object name is test_program
    uint offset = pio_add_program(pio, &test_program);

    // generated config function name
    pio_sm_config c = test_program_get_default_config(offset);

    // map "set pins" to GPIO25
    sm_config_set_set_pins(&c, LED, 1);

    // route GPIO25 to PIO
    pio_gpio_init(pio, LED);

    // make GPIO25 output
    pio_sm_set_consecutive_pindirs(pio, sm, LED, 1, true);
    sm_config_set_clkdiv(&c, 125.0f);
    // apply config
    pio_sm_init(pio, sm, offset, &c);

    // start PIO state machine
    pio_sm_set_enabled(pio, sm, true) ;
    pio_sm_put_blocking(pio, sm, 1000000);
    while (1) {
        tight_loop_contents();
    }
}