#include "hw_config.h"

// Define the SPI configuration profile matching your schematic
static spi_t spis[] = {
    {
        .hw_inst = spi0,           // Matches GPIO2/3/4 hardware block
        .miso_gpio = 4,            // MISO linked to GPIO4
        .mosi_gpio = 3,            // MOSI linked to GPIO3
        .sck_gpio = 2,             // SCK linked to GPIO2
        .baud_rate = 12500 * 1000  // SPI Speed: 12.5 MHz
    }
};

// Define the logical SD Card instance linked to the SPI configuration profile
static sd_card_t sd_cards[] = {
    {
        .pcName = "0:",            // FatFs logical drive identifier
        .spi = spis,               // FIXED: Removed '&' so it passes a clean pointer
        .ss_gpio = 5,              // CS linked to GPIO5
        .use_card_detect = false   // No dedicated card detect pin shown
    }
};

// Driver binding functions required by the Carlk3 framework
// FIXED: Adjusted sizeof math to target the first array indices [0]
size_t sd_get_num(void) { return sizeof(sd_cards) / sizeof(sd_cards[0]); }
sd_card_t *sd_get_by_num(size_t num) { return &sd_cards[num]; }
size_t spi_get_num(void) { return sizeof(spis) / sizeof(spis[0]); }
spi_t *spi_get_by_num(size_t num) { return &spis[num]; }

