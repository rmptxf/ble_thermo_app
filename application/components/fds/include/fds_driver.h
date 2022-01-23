#ifndef FDS_DRIVER_H__
#define FDS_DRIVER_H__

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

void f_storage_init(void);
void retreive_saved_presets(void);
void update_saved_presets(void);

#ifdef __cplusplus
}
#endif

#endif /*FDS_DRIVER_H__*/

