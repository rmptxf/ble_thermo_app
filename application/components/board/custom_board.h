/**
 * Copyright (c) 2016 - 2019, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef BOARD_CUSTOM_H
#define BOARD_CUSTOM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_gpio.h"

// BUTTONs definitions
#define BUTTONS_NUMBER 1

#define BUTTON_DETECTION_TIME 50

#define BUTTON_1       NRF_GPIO_PIN_MAP(0,4)
#define BSP_BUTTON_0   BUTTON_1

#define BUTTON_PULL    NRF_GPIO_PIN_NOPULL  //NOTE: Got to verify

#define BUTTONS_ACTIVE_STATE 0

#define BUTTONS_LIST { BUTTON_1 }

// LEDs definitions
#define RGB_LEDS_PIN      NRF_GPIO_PIN_MAP(0,16)
#define RGB_LEDS_COUNT    6

#define TEMPERATURE_SENSOR_PIN  NRF_SAADC_INPUT_AIN3
#define BATTERY_INPUT_PIN       NRF_SAADC_INPUT_AIN1

#define RX_PIN_NUMBER  NRF_GPIO_PIN_MAP(0,26)
#define TX_PIN_NUMBER  NRF_GPIO_PIN_MAP(0,27)
#define CTS_PIN_NUMBER NRF_GPIO_PIN_MAP(0,6)  // Not used
#define RTS_PIN_NUMBER NRF_GPIO_PIN_MAP(0,7)  // Not used
#define HWFC           true


#ifdef __cplusplus
}
#endif

#endif // BOARD_CUSTOM_H
