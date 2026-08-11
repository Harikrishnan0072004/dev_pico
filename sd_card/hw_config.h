#ifndef HW_CONFIG_H_
#define HW_CONFIG_H_

#include "ff.h"
#include "diskio.h"
#include "rtc.h"
#include "sd_card.h"

#ifdef __cplusplus
extern "C" {
#endif

// Library requirement: functions to retrieve the hardware configurations
size_t sd_get_num(void);
sd_card_t *sd_get_by_num(size_t num);
size_t spi_get_num(void);
spi_t *spi_get_by_num(size_t num);

#ifdef __cplusplus
}
#endif

#endif

