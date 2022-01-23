#ifndef SENSOR_DRIVER_H__
#define SENSOR_DRIVER_H__

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

void temperature_sensor_meas_init(void);
void temperature_sensor_meas_start(void);

#ifdef __cplusplus
}
#endif

#endif /*SENSOR_DRIVER_H__*/