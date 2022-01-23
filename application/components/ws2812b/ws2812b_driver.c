#include "ws2812b_driver.h"
#include "nrf_drv_pwm.h"
#include "nrf_delay.h"
#include "boards.h"

#define PIXEL_BIT_SIZE           ( 24u )
#define WS2812B_PWM_T            ( 20u )              /*< 20 ticks @ 16 MHz equals 1250 ns. */
#define WS2812B_1                ( 15u | 0x8000 )     /*< 15 ticks @ 16 MHz equals 937.5 ns, the MSB defines the PWM polarity during the sequence. */
#define WS2812B_0                ( 6u | 0x8000 )      /*< 6 ticks @ 16 MHz equals 375 ns, the MSB defines the PWM polarity during the sequence. */
#define WS2812B                  RGB_LEDS_PIN
#define WS2812B_RESET_LOW        45

/*Private variables*/
static WS2812B_StatusTypeDef WS2812B_State = WS2812B_RESET; 
static nrf_drv_pwm_t m_pwm0 = NRF_DRV_PWM_INSTANCE(WS2812B_PWM_INSTANCE);
static nrf_pwm_values_common_t WS2812B_Buffer[WS2812B_RESET_LOW+PIXEL_COUNT*PIXEL_BIT_SIZE];
nrf_pwm_sequence_t const seq =
{
    .values.p_common = WS2812B_Buffer,
    .length          = NRF_PWM_VALUES_LENGTH(WS2812B_Buffer),
    .repeats         = 0,
    .end_delay       = 0
};


/**@brief Initialisation of the periphherals for using the ws2812b leds.
 *
 */
WS2812B_StatusTypeDef ws2812b_init(void)
{
   // init pins
   nrf_gpio_cfg_output(WS2812B);
   nrf_gpio_pin_clear(WS2812B);
  
   // the end of the data buffer needs to set the output low for at least 50 us,
   // so the leds will accept their bit shiftregisters. This is done by 0 duty
   // cycles for WS2812B_RESET_LOW times.
   for(uint8_t i=0; i<WS2812B_RESET_LOW; i++)
   {
      WS2812B_Buffer[PIXEL_COUNT*PIXEL_BIT_SIZE+i] = 0x8000;
   }

    nrf_drv_pwm_config_t const config0 =
    {
        .output_pins =
        {
            WS2812B, // channel 0, if you would OR with "NRF_DRV_PWM_PIN_INVERTED", the pwm idle state would be high
            NRF_DRV_PWM_PIN_NOT_USED,             // channel 1
            NRF_DRV_PWM_PIN_NOT_USED,             // channel 2
            NRF_DRV_PWM_PIN_NOT_USED,             // channel 3
        },
        .irq_priority = APP_IRQ_PRIORITY_LOWEST,
        .base_clock   = NRF_PWM_CLK_16MHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = WS2812B_PWM_T,
        .load_mode    = NRF_PWM_LOAD_COMMON,
        .step_mode    = NRF_PWM_STEP_AUTO
    };
    APP_ERROR_CHECK(nrf_drv_pwm_init(&m_pwm0, &config0, NULL)); //WS2812B_pwmhandler
    
    WS2812B_State = WS2812B_READY;
    
    return WS2812B_OK;
}

/**@brief Function to send a buffer to the ws2812b leds.
 *
 */
void ws2812b_sendBuffer(void)
{
   /*WS2812B_State = WS2812B_BUSY;*/
  
   nrf_drv_pwm_simple_playback(&m_pwm0, &seq, 1, NRF_DRV_PWM_FLAG_STOP);
    
   /*while( WS2812B_State != WS2812B_READY );*/
}

/**@brief Function to clear the ws2812b leds buffer.
 *
 */
void ws2812b_clearBuffer(void)
{
   for( uint16_t i=0; i<PIXEL_COUNT*PIXEL_BIT_SIZE; i++ )
   {
      WS2812B_Buffer[i] = WS2812B_0;
   }
}

/**@brief Function used to set a pixel on the ws2812b led.
 *
 */
void ws2812b_setPixel(uint16_t pixel_pos, uint8_t red, uint8_t green, uint8_t blue)
{
  for(uint8_t i=0; i<8; i++)
  {
    if((0x80 & (red<<i)) == 0x80)
    {
      WS2812B_Buffer[pixel_pos*PIXEL_BIT_SIZE+8+i] = WS2812B_1;
    }
    else
    {
      WS2812B_Buffer[pixel_pos*PIXEL_BIT_SIZE+8+i] = WS2812B_0;
    }
    if( (0x80 & (green<<i)) == 0x80 )
    {
      WS2812B_Buffer[pixel_pos*PIXEL_BIT_SIZE+i] = WS2812B_1;
    }
    else
    {
      WS2812B_Buffer[pixel_pos*PIXEL_BIT_SIZE+i] = WS2812B_0;
    }
    if( (0x80 & (blue<<i)) == 0x80 )
    {
      WS2812B_Buffer[pixel_pos*PIXEL_BIT_SIZE+16+i] = WS2812B_1;
    }
    else
    {
      WS2812B_Buffer[pixel_pos*PIXEL_BIT_SIZE+16+i] = WS2812B_0;
    }
  }
}