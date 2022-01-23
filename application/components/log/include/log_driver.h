
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

void log_init(void);
void nrf_log_hex(const uint8_t *buffer, uint16_t len);