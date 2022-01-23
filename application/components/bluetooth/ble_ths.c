
#include "ble_ths.h"

/* Private functions */
static void on_connect(ble_ths_t * p_ths, ble_evt_t const * p_ble_evt);
static void on_disconnect(ble_ths_t * p_ths, ble_evt_t const * p_ble_evt);
static void on_write(ble_ths_t * p_ths, ble_evt_t const * p_ble_evt);

/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_ths       ths Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
void on_connect(ble_ths_t * p_ths, ble_evt_t const * p_ble_evt)
{
    p_ths->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}

/**@brief Function for handling the Disconnect event.
 *
 * @param[in]   p_ths       ths Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
void on_disconnect(ble_ths_t * p_ths, ble_evt_t const * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_ths->conn_handle = BLE_CONN_HANDLE_INVALID;
}

/**@brief Function for handling the Write event.
 *
 * @param[in]   p_ths       ths service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_write(ble_ths_t * p_ths, ble_evt_t const * p_ble_evt)
{
   ble_gatts_evt_write_t const * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
   ble_ths_evt_t evt;

   // writing to the command characteristic
   if ( p_evt_write->handle == p_ths->temperature_value_char_handles.value_handle)
   { 
        evt.params_command.command_data.p_data = p_evt_write->data;
        evt.params_command.command_data.length = p_evt_write->len;
        evt.evt_type = COMMAND_VALUE_CHAR_EVT_RX; 

        p_ths->evt_handler(p_ths, &evt);
   }

   // writing to the temperature characteristic (cccd)
   else if (p_evt_write->handle == p_ths->temperature_value_char_handles.cccd_handle)
   {
      if (ble_srv_is_notification_enabled(p_evt_write->data))
      {
          evt.evt_type = TEMPERATURE_VALUE_CHAR_EVT_NOTIFICATION_ENABLED;
      }
      else
      {
          evt.evt_type = TEMPERATURE_VALUE_CHAR_EVT_NOTIFICATION_DISABLED;
      }

      p_ths->evt_handler(p_ths, &evt);
   }
}

/**@brief Function for handling the thstom servie ble events.
 *
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
void ble_ths_on_ble_evt( ble_evt_t const * p_ble_evt, void * p_context)
{
    ble_ths_t * p_ths = (ble_ths_t *) p_context;

    if (p_ths == NULL || p_ble_evt == NULL)
    {
        return ;
    }

    switch (p_ble_evt->header.evt_id)
    {
      case BLE_GAP_EVT_CONNECTED:
          on_connect(p_ths, p_ble_evt);
          break;

      case BLE_GAP_EVT_DISCONNECTED:
          on_disconnect(p_ths, p_ble_evt);
          break;

      case BLE_GATTS_EVT_WRITE:
          on_write(p_ths, p_ble_evt);
          break;
      default:
          break;
    }
}


/**@brief Function for adding the temperature characteristic.
 *
 * @param[in]   p_ths        ths Service structure.
 * @param[in]   p_ths_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t temperature_char_add(ble_ths_t * p_ths, const ble_ths_init_t * p_ths_init)
{
    uint32_t                    err_code;
    ble_uuid_t                  ble_uuid;
    ble_add_char_params_t       add_char_params;

    uint8_t init_value [TEMPERATURE_VALUE_LEN] = {0};

    memset(&add_char_params, 0, sizeof(add_char_params));
    add_char_params.uuid             = TEMPERATURE_CHAR_UUID;
    add_char_params.uuid_type        = p_ths->uuid_type;

    add_char_params.init_len         = TEMPERATURE_VALUE_LEN;
    add_char_params.max_len          = TEMPERATURE_VALUE_LEN;
    add_char_params.p_init_value     = init_value;

    add_char_params.char_props.read   = 1;
    add_char_params.char_props.write  = 0;
    add_char_params.char_props.notify = 1;

    add_char_params.read_access  = SEC_OPEN;
    add_char_params.cccd_write_access = SEC_OPEN;

    err_code = characteristic_add(p_ths->service_handle, 
                                  &add_char_params, 
                                  &p_ths->temperature_value_char_handles);

    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    return NRF_SUCCESS;
}


/**@brief Function for adding the command characteristic.
 *
 * @param[in]   p_ths        ths Service structure.
 * @param[in]   p_ths_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t command_char_add(ble_ths_t * p_ths, const ble_ths_init_t * p_ths_init)
{
    uint32_t                  err_code;
    ble_uuid_t                ble_uuid;
    ble_add_char_params_t     add_char_params;

    uint8_t init_value [COMMAND_VALUE_LEN] = {0};

    memset(&add_char_params, 0, sizeof(add_char_params));
    add_char_params.uuid             = COMMAND_CHAR_UUID;
    add_char_params.uuid_type        = p_ths->uuid_type;

    add_char_params.init_len         = COMMAND_VALUE_LEN;
    add_char_params.max_len          = COMMAND_VALUE_LEN;
    add_char_params.p_init_value     = init_value;

    add_char_params.char_props.read   = 1;
    add_char_params.char_props.write  = 1;
    add_char_params.char_props.notify = 0;

    add_char_params.read_access  = SEC_OPEN;
    add_char_params.cccd_write_access = SEC_OPEN;

    err_code = characteristic_add(p_ths->service_handle, 
                                  &add_char_params, 
                                  &p_ths->command_value_char_handles);

    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    return NRF_SUCCESS;
}


/**@brief Function for initializing the thstom ble service.
 *
 * @param[in]   p_ths       thstom service structure.
 * @param[in]   p_ths_init  Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */

uint32_t ble_ths_init(ble_ths_t * p_ths, const ble_ths_init_t * p_ths_init)
{
    if (p_ths == NULL || p_ths_init == NULL)
    {
        return NRF_ERROR_NULL;
    }

    uint32_t err_code;
    ble_uuid_t ble_uuid;

    // Initialize service structure
    p_ths->evt_handler = p_ths_init->evt_handler;
    p_ths->conn_handle = BLE_CONN_HANDLE_INVALID;

    // Add ths Service UUID
    ble_uuid128_t base_uuid = THS_SERVICE_UUID_BASE;

    err_code = sd_ble_uuid_vs_add(&base_uuid, &p_ths->uuid_type);
    if (err_code != NRF_SUCCESS) return err_code;

    ble_uuid.type = p_ths->uuid_type;
    ble_uuid.uuid = THS_SERVICE_UUID;

    // Add the ths Service
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_ths->service_handle);
    if (err_code != NRF_SUCCESS) return err_code;

    // Add temperature characteristic
    err_code =  temperature_char_add(p_ths, p_ths_init);
    if (err_code != NRF_SUCCESS) return err_code;

    // Add command characteristic
    err_code =  command_char_add(p_ths, p_ths_init);
    if (err_code != NRF_SUCCESS) return err_code;

    return NRF_SUCCESS;
}

/**@brief Function for updating the temperature value.
 *
 * @param[in]   p_ths          ths Service structure.
 * @param[in]   p_temp         pointer to the temperature value.
 *
 * @return      NRF_SUCCESS  on success, otherwise an error code.
 */
uint32_t ble_ths_temperature_value_update(ble_ths_t * p_ths, uint8_t * p_temp_value)
{
    NRF_LOG_INFO("BLE Temperature value update.");
    nrf_log_hex(p_temp_value, TEMPERATURE_VALUE_LEN);

    if (p_ths == NULL)
    {
        return NRF_ERROR_NULL;
    }

    uint32_t err_code = NRF_SUCCESS;
    ble_gatts_value_t gatts_value;

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = TEMPERATURE_VALUE_LEN;
    gatts_value.offset  = 0;
    gatts_value.p_value = p_temp_value;

    // Update database.
    err_code = sd_ble_gatts_value_set(p_ths->conn_handle,
                                      p_ths->temperature_value_char_handles.value_handle,
                                      &gatts_value);
    if (err_code != NRF_SUCCESS) return err_code;

    // Send value if connected and notifying.
    if ((p_ths->conn_handle != BLE_CONN_HANDLE_INVALID))
    {
        ble_gatts_hvx_params_t hvx_params;

        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = p_ths->temperature_value_char_handles.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = gatts_value.offset;
        hvx_params.p_len  = &gatts_value.len;
        hvx_params.p_data = gatts_value.p_value;

        err_code = sd_ble_gatts_hvx(p_ths->conn_handle, &hvx_params);
        NRF_LOG_DEBUG("sd_ble_gatts_hvx result: %x.", err_code);
    }
    else
    {
        err_code = NRF_ERROR_INVALID_STATE;
        NRF_LOG_INFO("sd_ble_gatts_hvx result: NRF_ERROR_INVALID_STATE.");
    }

    return err_code;
}