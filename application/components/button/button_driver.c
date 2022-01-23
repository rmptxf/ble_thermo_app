#include "button_driver.h"
#include "power_mng.h"
#include "app_timer.h"
#include "boards.h"
#include "bsp.h"

APP_TIMER_DEF(m_btn_lp_timer_id); 
APP_TIMER_DEF(m_btn_dp_timer_id);

#define BTN_LP_TIME_MS  1000
#define BTN_DP_TIME_MS  1000

/*@brief Function for handeling buttons events.
 *
 */
static void button_event_handler(uint8_t pin_no, uint8_t button_action)
{
  ret_code_t err_code;

  switch(pin_no)
  {
   case BUTTON_1:
     if(button_action == APP_BUTTON_PUSH) 
     {

     }
     else if(button_action == APP_BUTTON_RELEASE) 
     {
 
     }
     break;

     default:
        return;
  }

}

/**@brief Function for buttons configuration.
 *
 */
static const app_button_cfg_t app_buttons[BUTTONS_NUMBER] = 
{
   {BUTTON_1, BUTTONS_ACTIVE_STATE, BUTTON_PULL, button_event_handler}
}; 


/**@brief Function for initializing buttons.
 *
 */
void buttons_init(void)
{

    ret_code_t  err_code;                        

    err_code = app_button_init((app_button_cfg_t *)app_buttons,
                                                BUTTONS_NUMBER,
                                         BUTTON_DETECTION_TIME);
    APP_ERROR_CHECK(err_code);

    err_code = app_button_enable();
    APP_ERROR_CHECK(err_code);
}

