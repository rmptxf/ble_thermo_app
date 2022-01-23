#ifndef BLUETOOTH_DRIVER_H__
#define BLUETOOTH_DRIVER_H__

#include <stdint.h>
#include <stdbool.h>

#include "nrf_sdh.h"
#include "ble_gap.h"
#include "nrf_sdh_ble.h"
#include "nrf_ble_qwr.h"
#include "nrf_ble_gatt.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "boards.h"
#include "fds.h"
#include "app_timer.h"
#include "ble_ths.h"
#include "ble_bas.h"
#include "log_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

void bluetooth_init(void);
void timers_init(void);
uint32_t restart_adv_without_whitelist(void);
void advertising_start(void);
uint32_t advertising_stop(void);
bool is_connected(void);
void disconnect(void);
void ble_temperature_value_update(float temperature);
void ble_battery_level_value_update(uint8_t battery_level);

#ifdef __cplusplus
}
#endif

#endif /* BLUETOOTH_DRIVER_H__ */