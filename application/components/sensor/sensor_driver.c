#include "sensor_driver.h"
#include "bluetooth_driver.h"
#include "adc_driver.h"
#include "app_timer.h"
#include "log_driver.h"

APP_TIMER_DEF(m_temperature_timer_id);
#define TEMPERATURE_MEAS_INTERVAL_MS  30000

/**@brief Function for handling the Battery measurement timer timeout.
 *
 */
static void temperature_value_update(void *p_context)
{
    float temperature;
    temperature = get_temperature_value();
    
    ble_temperature_value_update(temperature);
}

/**@brief Function for initializing temperature sensor measurements.
 */
void temperature_sensor_meas_init(void)
{
    ret_code_t err_code;
    err_code = app_timer_create(&m_temperature_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                temperature_value_update);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("Temperature sensor measurements initialized");
}


/**@brief Function for starting temperature sensor measurements.
 */
void temperature_sensor_meas_start(void)
{
    ret_code_t err_code;

    err_code = app_timer_start(m_temperature_timer_id, 
                               APP_TIMER_TICKS(TEMPERATURE_MEAS_INTERVAL_MS),
                               NULL);
    APP_ERROR_CHECK(err_code);  
    
    NRF_LOG_INFO("Temperature sensor measurements started");     
}