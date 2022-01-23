#ifndef BLE_THS_H__
#define BLE_THS_H__

#include <string.h>
#include <stdint.h>
#include "ble.h"
#include "sdk_common.h"
#include "ble_srv_common.h"
#include "app_error.h"
#include "log_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BLE_THS_BLE_OBSERVER_PRIO  2

/**@brief   Macro for defining a ble_ths instance.
 *
 * @param   _name   Name of the instance.
 * @hideinitializer
 */
#define BLE_THS_DEF(_name)                                                            \
static ble_ths_t _name;                                                               \
NRF_SDH_BLE_OBSERVER(_name ## _obs,                                                   \
                     BLE_THS_BLE_OBSERVER_PRIO,                                       \
                     ble_ths_on_ble_evt, &_name)


#define THS_SERVICE_UUID_BASE   { 0x23, 0xD1, 0x15, 0xEF, 0x5F, 0x56, 0x23, 0x15, \
                                  0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00 }

#define THS_SERVICE_UUID          0x1400
#define TEMPERATURE_CHAR_UUID     0x1401 
#define COMMAND_CHAR_UUID         0x1402	
	
#define TEMPERATURE_VALUE_LEN     02
#define COMMAND_VALUE_LEN         01
        																	
/**@brief ths service event types.
 */
typedef enum
{   
    TEMPERATURE_VALUE_CHAR_EVT_NOTIFICATION_ENABLED,
    TEMPERATURE_VALUE_CHAR_EVT_NOTIFICATION_DISABLED,
    COMMAND_VALUE_CHAR_EVT_RX
} ble_ths_evt_type_t;


/**@brief This structure is passed to an event when @ref COMMAND_VALUE_CHAR_EVT_RX occurs.
 */
typedef struct 
{
    uint8_t const * p_data;   
    uint16_t   length;  
} command_data_t;


/**@brief ths Service event.
 */
typedef struct
{
   ble_ths_evt_type_t evt_type;                     

   union
   {
       command_data_t command_data;
       
   } params_command;

} ble_ths_evt_t;


/**@brief Forward declaration of the ble_ths_t type.
 */
typedef struct ble_ths_s ble_ths_t;


/**@brief Csutom service event handler type.
 */
typedef void (*ble_ths_evt_handler_t) (ble_ths_t * p_ths, ble_ths_evt_t * p_evt);


/**@brief thstom Service init structure. This contains all options and data needed for
 *        initialization of the service.*/
typedef struct
{
    ble_ths_evt_handler_t         evt_handler;                   /**< Event handler to be called for handling events in the thstom Service. */  
   
    ble_srv_cccd_security_mode_t  temperature_char_attr_md;      /**< Initial security level for the temperature characteristic attribute */
    ble_srv_cccd_security_mode_t  command_char_attr_md;          /**< Initial security level for the command characteristic attribute */
} ble_ths_init_t;


/**@brief ths Service structure. This contains various status information for the service. */
struct ble_ths_s
{
    ble_ths_evt_handler_t         evt_handler;                    /**< Event handler to be called for handling events in the thstom Service. */
    uint16_t                      service_handle;                 /**< Handle of thstom Service (as provided by the BLE stack). */
   
    ble_gatts_char_handles_t      temperature_value_char_handles; /**< Handles related to the temperature characteristic. */
    ble_gatts_char_handles_t      command_value_char_handles;     /**< Handles related to the command characteristic. */
     
    uint16_t  conn_handle;                                        /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
    uint8_t   uuid_type;                                          /**< UUID Type. */
};

/**@brief Function for initializing the ths ble service.
 *
 * @param[in]   p_ths       ths service structure.
 * @param[in]   p_ths_init  Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t ble_ths_init(ble_ths_t * p_ths, const ble_ths_init_t * p_ths_init);


/**@brief Function for handling the thstom service ble events.
 *
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
void ble_ths_on_ble_evt( ble_evt_t const * p_ble_evt, void * p_context);


/**@brief Function for updating the temperature value.
 *
 * @param[in]   p_ths           ths Service structure.
 * @param[in]   p_temp_value    the temperature value pointer.
 *
 * @return      NRF_SUCCESS  on success, otherwise an error code.
 */
uint32_t ble_ths_temperature_value_update(ble_ths_t * p_ths, uint8_t * p_temp_value);

#endif /*BLE_THS_H__*/