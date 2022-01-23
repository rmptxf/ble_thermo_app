#ifndef WS2812B_H__
#define WS2812B_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define PIXEL_COUNT    RGB_LEDS_COUNT

typedef enum
{
   WS2812B_OK       = 0x00U,
   WS2812B_ERROR    = 0x01U,
   WS2812B_BUSY     = 0x02U,
   WS2812B_TIMEOUT  = 0x03U,
   WS2812B_READY    = 0x04U,
   WS2812B_RESET    = 0x05U
} WS2812B_StatusTypeDef;

WS2812B_StatusTypeDef ws2812b_init(void);
void ws2812b_sendBuffer(void);
void ws2812b_clearBuffer(void);
void ws2812b_setPixel(uint16_t pixel_pos, uint8_t red, uint8_t green, uint8_t blue);

#ifdef __cplusplus
}
#endif

#endif // WS2812B_H__