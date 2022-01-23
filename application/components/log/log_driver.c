
#include "log_driver.h"


/**@brief Function for initializing the nrf log module.
 */
void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

/**@brief Function for printing hex byte array.
 *
 * @param[in]   buffer      input buffer required to be printed.
 * @param[in]   len         length of buffer.
 */
void nrf_log_hex(const uint8_t *buffer, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
    {
        NRF_LOG_RAW_INFO("0x%02x ", buffer[i]);
        if ((i % 16 == 0) && i)
            NRF_LOG_RAW_INFO("\n");
    }
    NRF_LOG_RAW_INFO("\n");
}