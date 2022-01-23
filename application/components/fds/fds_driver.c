#include "fds_driver.h"
#include "fds.h"
#include "power_mng.h"
#include "log_driver.h"

#define CONFIG_FILE     (0x8010)
#define CONFIG_REC_KEY  (0x7010)

typedef struct
{
    uint8_t      presets[5]; 

} rec_info_t;

static __ALIGN(4) rec_info_t m_record =
{
    .presets = {20, 30, 40, 50, 0}
};

/**@brief A record containing temperature presets data. */
static fds_record_t const m_file_record =
{
    .file_id           = CONFIG_FILE,
    .key               = CONFIG_REC_KEY,
    .data.p_data       = &m_record,
    /* The length of a record is always expressed in 4-byte units (words). */
    .data.length_words = (sizeof(m_record) + 3) / sizeof(uint32_t),
};

/**@brief Array to map FDS events to strings. */
static char const * fds_evt_str[] =
{
    "FDS_EVT_INIT",
    "FDS_EVT_WRITE",
    "FDS_EVT_UPDATE",
    "FDS_EVT_DEL_RECORD",
    "FDS_EVT_DEL_FILE",
    "FDS_EVT_GC"
};

/**@brief Keep track of the progress of a delete_all operation. */
static struct
{
    bool delete_next;   
    bool pending;     
} m_delete_all;

/* Flag to check fds initialization. */
static bool volatile m_fds_initialized = false;

const char *fds_err_str(ret_code_t ret)
{
  /* Array to map FDS return values to strings. */
  static char const * err_str[] =
  {
      "FDS_ERR_OPERATION_TIMEOUT",
      "FDS_ERR_NOT_INITIALIZED",
      "FDS_ERR_UNALIGNED_ADDR",
      "FDS_ERR_INVALID_ARG",
      "FDS_ERR_NULL_ARG",
      "FDS_ERR_NO_OPEN_RECORDS",
      "FDS_ERR_NO_SPACE_IN_FLASH",
      "FDS_ERR_NO_SPACE_IN_QUEUES",
      "FDS_ERR_RECORD_TOO_LARGE",
      "FDS_ERR_NOT_FOUND",
      "FDS_ERR_NO_PAGES",
      "FDS_ERR_USER_LIMIT_REACHED",
      "FDS_ERR_CRC_CHECK_FAILED",
      "FDS_ERR_BUSY",
      "FDS_ERR_INTERNAL",
  };

  return err_str[ret - NRF_ERROR_FDS_ERR_BASE];
}

/**@brief Array to map FDS return values to strings. */
static void fds_evt_handler(fds_evt_t const * p_evt)
{
    if (p_evt->result == NRF_SUCCESS)
    {
        NRF_LOG_INFO("Event: %s received (NRF_SUCCESS)",
                      fds_evt_str[p_evt->id]);
    }
    else
    {
        NRF_LOG_INFO("Event: %s received (%s)",
                      fds_evt_str[p_evt->id],
                      fds_err_str(p_evt->result));
  
    }

    switch (p_evt->id)
    {
        case FDS_EVT_INIT:
            if (p_evt->result == NRF_SUCCESS)
            {
                m_fds_initialized = true;
            }
            break;

        case FDS_EVT_WRITE:
        {
            if (p_evt->result == NRF_SUCCESS)
            {
                NRF_LOG_INFO("Record ID:\t0x%04x",  p_evt->write.record_id);
                NRF_LOG_INFO("File ID:\t0x%04x",    p_evt->write.file_id);
                NRF_LOG_INFO("Record key:\t0x%04x", p_evt->write.record_key);
            }
        } break;

        case FDS_EVT_DEL_RECORD:
        {
            if (p_evt->result == NRF_SUCCESS)
            {
                NRF_LOG_INFO("Record ID:\t0x%04x",  p_evt->del.record_id);
                NRF_LOG_INFO("File ID:\t0x%04x",    p_evt->del.file_id);
                NRF_LOG_INFO("Record key:\t0x%04x", p_evt->del.record_key);
            }
            m_delete_all.pending = false;
        } break;

        default:
            break;
    }
}

/**@brief  Wait for fds to initialize. */
static void wait_for_fds_ready(void)
{
    while (!m_fds_initialized)
    {
        power_manage();
    }
}

/**@brief function for intitialising the fds. */
void f_storage_init(void)
{
    ret_code_t rc;

    /* Register first to receive an event when initialization is complete. */
    (void) fds_register(fds_evt_handler);

    rc = fds_init();
    APP_ERROR_CHECK(rc);

    /* Wait for fds to initialize. */
    wait_for_fds_ready();

    fds_stat_t stat = {0};

    rc = fds_stat(&stat);
    APP_ERROR_CHECK(rc);

    NRF_LOG_INFO("Reading flash usage statistics...");
    NRF_LOG_INFO("Found %d valid records.", stat.valid_records);
    NRF_LOG_INFO("Found %d dirty records (ready to be garbage collected).", stat.dirty_records);
}

/**@brief function to retreive saved temperature presets. */
void retreive_saved_presets(void)
{ 
    ret_code_t rc;

    fds_record_desc_t desc = {0};
    fds_find_token_t  tok  = {0};

    rc = fds_record_find(CONFIG_FILE, CONFIG_REC_KEY, &desc, &tok);

    if (rc == NRF_SUCCESS)
    {
        /* A config file is in flash. Let's update it. */
        fds_flash_record_t config = {0};

        /* Open the record and read its contents. */
        rc = fds_record_open(&desc, &config);
        APP_ERROR_CHECK(rc);

        /* Copy the record_infos file from flash. */
        memcpy(&m_record, config.p_data, sizeof(rec_info_t));

        NRF_LOG_INFO("presets record found.");
        for(uint8_t i=0; i<sizeof(m_record.presets)-1;i++)
        {
           NRF_LOG_INFO("presets[%d] : %d C.", i, m_record.presets[i]);
        }
        NRF_LOG_INFO("last used preset : %d C.", m_record.presets[m_record.presets[4]]);

        /* Close the record when done reading. */
        rc = fds_record_close(&desc);
        APP_ERROR_CHECK(rc);
    }
    else
    {
        NRF_LOG_INFO("presets record not found.");
        NRF_LOG_INFO("writing a new presets record ...");

        rc = fds_record_write(&desc, &m_file_record);
        if(rc == NRF_SUCCESS)
        {
            NRF_LOG_INFO("File written successfully.");        
        }
        else if (rc == FDS_ERR_NO_SPACE_IN_FLASH)
        {
            NRF_LOG_INFO("No space in flash, delete some records to update the config file.");
            fds_gc(); // runing garbage collection
        }
    }
}

/**@brief function for updating the last record file info. */
void update_saved_presets(void)
{
    ret_code_t rc;

    fds_record_desc_t desc = {0};
    fds_find_token_t  tok  = {0};

    rc = fds_record_find(CONFIG_FILE, CONFIG_REC_KEY, &desc, &tok);

    if (rc == NRF_SUCCESS)
     {
        /* Write the updated record to flash. */
        rc = fds_record_update(&desc, &m_file_record);
        if(rc == NRF_SUCCESS)
        {
           NRF_LOG_INFO("presets were successfully updated");
        }
        else if (rc == FDS_ERR_NO_SPACE_IN_FLASH)
        {
            NRF_LOG_INFO("No space in flash, delete some records to update the config file.");
            rc = fds_gc(); // runing garbage collection
        }
     }
}
