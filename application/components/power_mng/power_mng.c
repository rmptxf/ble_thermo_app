
#include "power_mng.h"
#include "nrf_pwr_mgmt.h"
#include "bsp_btn_ble.h"
#include "app_timer.h"
#include "log_driver.h"

#define SLEEP_DELAY_MS  100
APP_TIMER_DEF(m_sleepmode_enter_timer_id);

/**@brief Function for putting the chip into sleep mode.
 *
 * @note This function will not return.
 */
static void entre_sleep_pmode(void * p_context)
{
    ret_code_t err_code;

    err_code = bsp_indication_set(BSP_INDICATE_IDLE);
    APP_ERROR_CHECK(err_code);

    // Prepare wakeup buttons.
    err_code = bsp_btn_ble_sleep_mode_prepare();
    APP_ERROR_CHECK(err_code);

    // Go to system-off mode (this function will not return; wakeup will cause a reset).
    err_code = sd_power_system_off();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing power management.
 */
void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(&m_sleepmode_enter_timer_id,
                                APP_TIMER_MODE_SINGLE_SHOT,
                                entre_sleep_pmode);

    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling the idle state (main loop).
 *
 * @details If there is no pending log operation, then sleep until next the next event occurs.
 */
void idle_state_handle(void)
{
    if (NRF_LOG_PROCESS() == false)
    {
        nrf_pwr_mgmt_run();
    }
}


/**@brief Function for making the systme enters the sleep mode.
 *
 */
void sleep_mode_enter(void)
{
    ret_code_t err_code;
    err_code = app_timer_start(m_sleepmode_enter_timer_id,
                              APP_TIMER_TICKS(SLEEP_DELAY_MS),
                              NULL);

    APP_ERROR_CHECK(err_code);
}


/**@brief Function for managing power.
 *
 */
void power_manage(void)
{
    nrf_pwr_mgmt_run();
}
