# PCF8574 I2C LCD Test (Pico SDK)

## Wiring
| PCF8574 backpack | Pico pin |
|---|---|
| VCC | VBUS (5V) or 3V3 — check your backpack's logic level, most tolerate both |
| GND | GND |
| SDA | GPIO6 |
| SCL | GPIO7 |

Pull-ups: the code enables the Pico's internal pull-ups on SDA/SCL. Most
backpacks also have their own onboard pull-ups, so this should work either
way, but if the bus scan finds nothing, try removing `gpio_pull_up()` calls
in case of double pull-up loading, or check continuity on SDA/SCL first.

## Build (WSL, standard Pico SDK / CMake flow)

You'll need `pico_sdk_import.cmake` — copy it from your SDK install:

```bash
cp $PICO_SDK_PATH/external/pico_sdk_import.cmake .
mkdir build && cd build
cmake -DPICO_BOARD=pico ..
make -j4
```

Flash `i2c_lcd_test.uf2` via BOOTSEL as usual.

## What to expect
1. Serial output (USB CDC) prints the I2C scan result first — confirm you
   see a device at `0x27` or `0x3F`. If you see nothing, it's a wiring or
   address issue before the LCD driver is even relevant.
2. If the address differs from `0x27`, update `LCD_I2C_ADDR` in
   `pcf8574_lcd.h`.
3. LCD should clear and print "Hello" at row 0, col 0.

## Common failure modes
- **Scan finds nothing**: SDA/SCL swapped, no pull-ups, or wrong voltage
  level on the backpack.
- **Scan finds device but LCD shows garbage/blocks**: contrast pot on the
  backpack needs adjusting (small blue trimpot) — this is unrelated to I2C
  and catches almost everyone the first time.
- **Scan finds device but nothing displays at all**: check the init timing
  — if you're running this on a much slower/faster clock setup than
  default, the `sleep_us`/`sleep_ms` delays in `lcd_init()` are the
  HD44780 datasheet minimums and shouldn't need changing, but worth
  knowing if you port this into your bare-metal boot chain later where
  `sleep_ms` depends on your SysTick/timer setup being correctly
  initialized first.
//picotool load -u -v -x -t elf i2c.elf