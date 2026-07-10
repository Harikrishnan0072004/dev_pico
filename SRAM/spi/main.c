/*
 * spi_test.c
 *
 * SPI peripheral sanity check for RP2040 using the Pico SDK.
 *
 * TEST METHOD: Physical loopback.
 * Short MOSI to MISO on the SPI instance under test with a jumper wire.
 * Whatever byte you clock out on MOSI should be shifted straight back in
 * on MISO. If tx[] == rx[] for every pattern below, the SSI peripheral,
 * clock divider, and GPIO function-select muxing are all working.
 *
 * Default pin map here is spi0 on the Pico's default SPI pins:
 *   GPIO 18 -> SCK
 *   GPIO 19 -> MOSI (TX)
 *   GPIO 16 -> MISO (RX)   <-- jumper this to GPIO 19
 *   GPIO 17 -> CS
 *
 * If you're testing spi1 or custom pins on your own board, just change
 * the #defines below.
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

#define SPI_PORT   spi0
#define PIN_SCK     2 //18
#define PIN_MOSI    3 //19
#define PIN_MISO    4 // 16   // physically jumper this to PIN_MOSI for loopback
#define PIN_CS     5

#define BAUD_HZ    1000000  // 1 MHz to start; bump up once basic test passes

static void print_hex_buf(const char *label, const uint8_t *buf, size_t len) {
    printf("%s: ", label);
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", buf[i]);
    }
    printf("\n");
}

// Runs one tx/rx pair through the SPI peripheral and checks it against
// the pattern that went out. Returns true on match.
static bool run_pattern(const uint8_t *pattern, size_t len) {
    uint8_t rx[32] = {0};
    if (len > sizeof(rx)) len = sizeof(rx);

    gpio_put(PIN_CS, 0);
    spi_write_read_blocking(SPI_PORT, pattern, rx, len);
    gpio_put(PIN_CS, 1);

    bool ok = (memcmp(pattern, rx, len) == 0);

    print_hex_buf("  TX", pattern, len);
    print_hex_buf("  RX", rx, len);
    printf("  -> %s\n", ok ? "MATCH" : "MISMATCH");

    return ok;
}

int main() {
    stdio_init_all();

    // Give USB CDC time to enumerate so early prints aren't lost.
    sleep_ms(3000);

    printf("\n=== RP2040 SPI Peripheral Test ===\n");
    printf("Using %s, SCK=%d MOSI=%d MISO=%d CS=%d, baud target %d Hz\n",
           (SPI_PORT == spi0) ? "spi0" : "spi1",
           PIN_SCK, PIN_MOSI, PIN_MISO, PIN_CS, BAUD_HZ);
    printf("Wire MOSI(GP%d) -> MISO(GP%d) for this test to pass.\n\n",
           PIN_MOSI, PIN_MISO);

    // --- Peripheral init ---
    uint actual_baud = spi_init(SPI_PORT, BAUD_HZ);
    spi_set_format(SPI_PORT,
                    8,              // 8 data bits
                    SPI_CPOL_0,     // clock polarity
                    SPI_CPHA_0,     // clock phase
                    SPI_MSB_FIRST);

    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);

    // CS is handled manually here (not GPIO_FUNC_SPI) so we control
    // the exact assert/deassert window around each transfer.
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1); // idle high

    printf("spi_init() returned actual baud = %u Hz\n\n", actual_baud);

    // --- Test patterns ---
    int pass = 0, total = 0;

    const uint8_t p1[] = {0x00};
    const uint8_t p2[] = {0xFF};
    const uint8_t p3[] = {0xA5};
    const uint8_t p4[] = {0xDE, 0xAD, 0xBE, 0xEF};
    const uint8_t p5[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

    struct { const uint8_t *data; size_t len; const char *name; } tests[] = {
        {p1, sizeof(p1), "all zeros"},
        {p2, sizeof(p2), "all ones"},
        {p3, sizeof(p3), "0xA5 alternating bits"},
        {p4, sizeof(p4), "DEADBEEF"},
        {p5, sizeof(p5), "walking bit"},
    };

    for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
        printf("Test %d: %s\n", (int)i + 1, tests[i].name);
        total++;
        if (run_pattern(tests[i].data, tests[i].len)) pass++;
        printf("\n");
        sleep_ms(200);
    }

    printf("=== Result: %d/%d patterns matched ===\n", pass, total);
    if (pass == total) {
        printf("SPI peripheral appears to be functioning correctly.\n");
    } else {
        printf("Mismatch detected. Check:\n");
        printf("  - Is MOSI actually jumpered to MISO?\n");
        printf("  - Correct GPIO numbers for your board (not physical pin numbers)?\n");
        printf("  - Any solder/continuity issue on the SPI lines (marginal joints)?\n");
        printf("  - Baud rate too high for your wiring (try lowering BAUD_HZ)?\n");
    }

    // Blink onboard behavior via stdio loop so you can reset/observe over USB.
    while (true) {
        printf(".");
        fflush(stdout);
        sleep_ms(2000);
    main();
    }
}