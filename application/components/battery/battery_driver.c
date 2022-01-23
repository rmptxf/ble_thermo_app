#include "battery_driver.h"
#include "bluetooth_driver.h"
#include "adc_driver.h"
#include "app_timer.h"
#include "log_driver.h"

APP_TIMER_DEF(m_battery_timer_id);
#define BATTERY_MEAS_INTERVAL_MS  30000

/**@brief Function for handling the Battery measurement timer timeout.
 *
 */
static void battery_level_update(void *p_context)
{
    uint8_t battery_level;
    battery_level = get_battery_level();
    
    ble_battery_level_value_update(battery_level);
}

/**@brief Function for initializing battery level measurements.
 */
void battery_meas_init(void)
{
    ret_code_t err_code;
    err_code = app_timer_create(&m_battery_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                battery_level_update);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("Battery measurements initialized");
}


/**@brief Function for starting battery level measurements.
 */
void battery_meas_start(void)
{
    ret_code_t err_code;

    err_code = app_timer_start(m_battery_timer_id, 
                               APP_TIMER_TICKS(BATTERY_MEAS_INTERVAL_MS),
                               NULL);
    APP_ERROR_CHECK(err_code);  
    
    NRF_LOG_INFO("Battery measurements started");     
}