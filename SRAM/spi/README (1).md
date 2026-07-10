# RP2040 SPI Peripheral Test

## What it does
Loopback self-test for the SPI peripheral: transmits several bit patterns
and checks that the same bytes come back on MISO. This isolates the SSI
peripheral + clock + GPIO mux from any external device, so a mismatch
means the issue is in the RP2040 itself or its wiring, not in whatever
you eventually attach.

## Wiring
Default pins (spi0):
- GPIO 18 = SCK
- GPIO 19 = MOSI
- GPIO 16 = MISO  <- jumper this to GPIO 19 (MOSI) with a wire
- GPIO 17 = CS (toggled manually, not required for loopback but included
  so the test looks like a real transaction)

Change the `#define PIN_*` values in `main.c` if you're testing spi1 or
custom pins on your own board.

## Build
```bash
git clone -b master https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk && git submodule update --init && cd ..
export PICO_SDK_PATH=$(pwd)/pico-sdk

mkdir build && cd build
cmake -DPICO_BOARD=pico ..
make -j4
```
This produces `spi_test.uf2` — hold BOOTSEL, plug in, drag the UF2 over,
then open a serial terminal (or `picocom`/`minicom` over the USB CDC port)
at any baud rate (USB CDC ignores the baud setting).

## Reading the output
Each test prints the TX pattern, the RX pattern, and MATCH/MISMATCH.
All five should MATCH if the peripheral and wiring are healthy.

## Adapting this for a real SPI device instead of loopback
If you actually want to test SPI against a device (e.g. an SPI/QSPI flash
chip, given your current board bring-up work) rather than loopback, the
most common first test is a JEDEC ID read, which needs no interpretation
of a full protocol — just 1 opcode byte out, 3 ID bytes back:

```c
uint8_t cmd = 0x9F; // JEDEC READ ID opcode, standard for most SPI NOR flash
uint8_t id[3] = {0};

gpio_put(PIN_CS, 0);
spi_write_blocking(SPI_PORT, &cmd, 1);
spi_read_blocking(SPI_PORT, 0x00, id, 3);
gpio_put(PIN_CS, 1);

printf("JEDEC ID: %02X %02X %02X\n", id[0], id[1], id[2]);
```
A sane non-`0xFF`/non-`0x00` manufacturer+device ID here confirms SPI
transactions are reaching the chip and coming back correctly — useful for
narrowing down whether a marginal QSPI joint affects standard single-line
SPI reads too.

## Troubleshooting a MISMATCH
- Confirm you used GPIO numbers, not physical pin numbers, when wiring.
- Lower `BAUD_HZ` (try 100000) — a marginal solder joint or long jumper
  wire can fail at 1 MHz but pass at slower clock rates, which tells you
  it's a signal-integrity issue rather than a dead peripheral.
- Check continuity with a multimeter on the jumper itself.
- Try the other SPI instance (spi1) on different pins to rule out one
  specific GPIO pad being damaged.
