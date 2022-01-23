#ifndef BATTERY_DRIVER__
#define BATTERY_DRIVER__

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

void battery_meas_init(void);
void battery_meas_start(void);


#ifdef __cplusplus
}
#endif

#endif /*BATTERY_DRIVER__ */