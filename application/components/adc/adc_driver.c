#include "adc_driver.h"
#include "nrf_drv_saadc.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "log_driver.h"
#include "boards.h"

#define ADC_LOG_ENABLED  1
#define SAMPLES_IN_BUFFER 2

static volatile float temperature = 0;
static volatile uint8_t  battery_level = 0;

static const nrf_drv_timer_t m_timer = NRF_DRV_TIMER_INSTANCE(SAADC_SAMPLING_TIMER_INSTANCE);
static nrf_saadc_value_t     m_buffer_pool[2][SAMPLES_IN_BUFFER];
static nrf_ppi_channel_t     m_ppi_channel;

// converting adc results to voltage value.
// RESULT = [V(P) – V(N) ] * GAIN/REFERENCE * 2(RESOLUTION - m)
// (m=0) if CONFIG.MODE=SE, or (m=1) if CONFIG.MODE=Diff.

// V(N) = 0;
// GAIN = 1/6;
// REFERENCE Voltage = internal (0.6V);
// RESOLUTION : 12 bit;
// m = 0;

// 12bit
// V(P) = RESULT x REFERENCE / ( GAIN x RESOLUTION) = RESULT x (600 / (1/6 x 2^(12)) =  ADC_RESULT x 0.87890625;
#define ADC_RESULT_IN_MILLI_VOLTS(ADC_RESULT) ((ADC_RESULT * 0.87890625))

/* Private functions */
static void saadc_sampling_event_init(void);
static void saadc_callback(nrf_drv_saadc_evt_t const * p_event);

/*@brief Function used to initialize the saadc sampling.
 *
 */
static void saadc_sampling_event_init(void)
{
    ret_code_t err_code;

    err_code = nrf_drv_ppi_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_32;
    err_code = nrf_drv_timer_init(&m_timer, &timer_cfg, NULL);
    APP_ERROR_CHECK(err_code);

    /* setup m_timer for compare event every 300ms */
    uint32_t ticks = nrf_drv_timer_ms_to_ticks(&m_timer, 300);
    nrf_drv_timer_extended_compare(&m_timer,
                                   NRF_TIMER_CC_CHANNEL0,
                                   ticks,
                                   NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK,
                                   false);
    nrf_drv_timer_enable(&m_timer);

    uint32_t timer_compare_event_addr = nrf_drv_timer_compare_event_address_get(&m_timer,
                                                                                NRF_TIMER_CC_CHANNEL0);
    uint32_t saadc_sample_task_addr   = nrf_drv_saadc_sample_task_get();

    /* setup ppi channel so that timer compare event is triggering sample task in SAADC */
    err_code = nrf_drv_ppi_channel_alloc(&m_ppi_channel);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_ppi_channel_assign(m_ppi_channel,
                                          timer_compare_event_addr,
                                          saadc_sample_task_addr);
    APP_ERROR_CHECK(err_code);
}

/*@brief Function used to handle the saadc callbacks.
 *
 */
static void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{ 
   ret_code_t err_code;

    if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
    {     
        uint16_t adc_channels_mv[2] = {0};

        err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, SAMPLES_IN_BUFFER);
        APP_ERROR_CHECK(err_code);

        for (uint8_t i = 0; i < SAMPLES_IN_BUFFER; i++)
        {
            adc_channels_mv[i] = ADC_RESULT_IN_MILLI_VOLTS(p_event->data.done.p_buffer[i]);
        }

        temperature = (adc_channels_mv[0] - 500)/10.0;
        if(temperature < 0) temperature = 0;

        battery_level = (adc_channels_mv[1]*2)/100;
#if ADC_LOG_ENABLED
        NRF_LOG_INFO("temperature : " NRF_LOG_FLOAT_MARKER " C°.", NRF_LOG_FLOAT(temperature));
        NRF_LOG_INFO("battery_level : %d.", battery_level);
#endif
    }
}

/*@brief Function used to initialize the saadc.
 *
 */
void saadc_init(void)
{
    ret_code_t err_code;
    nrf_saadc_channel_config_t channel0_config =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(TEMPERATURE_SENSOR_PIN);

    nrf_saadc_channel_config_t channel1_config =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(BATTERY_INPUT_PIN);

    err_code = nrf_drv_saadc_init(NULL, saadc_callback);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_channel_init(0, &channel0_config);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_channel_init(1, &channel1_config);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[0], SAMPLES_IN_BUFFER);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[1], SAMPLES_IN_BUFFER);
    APP_ERROR_CHECK(err_code);

    saadc_sampling_event_init();
}

/*@brief Function used to enable the saadc sampling.
 *
 */
void saadc_sampling_event_enable(void)
{
    ret_code_t err_code = nrf_drv_ppi_channel_enable(m_ppi_channel);

    APP_ERROR_CHECK(err_code);
}

/*@brief Function used to get the battery level.
 *
 */
uint8_t get_battery_level(void)
{
    return battery_level;
}

/*@brief Function used to get the temperature.
 *
 */
float get_temperature_value(void)
{
    return temperature;
}