#ifndef POWER_MNG_H__
#define POWER_MNG_H__

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

void power_management_init(void);
void idle_state_handle(void);
void sleep_mode_enter(void);
void power_manage(void);

#ifdef __cplusplus
}
#endif

#endif /*POWER_MNG_H__*/