#ifndef ADC_DRIVER__
#define ADC_DRIVER__

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif


void saadc_init(void);
void saadc_sampling_event_enable(void);
uint8_t get_battery_level(void);
float get_temperature_value(void);

#ifdef __cplusplus
}
#endif

#endif /*ADC_DRIVER__*/