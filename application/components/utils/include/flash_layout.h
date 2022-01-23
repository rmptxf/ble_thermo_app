/* ****************************************************************************
 * Copyright (C) Bojan Potočnik - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bojan Potočnik <info@bojanpotocnik.com>, 2020
 *************************************************************************** */

#ifndef FLASH_LAYOUT_H_INCLUDED
#define FLASH_LAYOUT_H_INCLUDED

#define FLASH_LAYOUT_AS7265X_FW_OFFSET      (0x00030000)
#define FLASH_LAYOUT_AS7265X_FW_START       ("ams\x01\x02")
#define FLASH_LAYOUT_AS7265X_FW_START_LEN   (sizeof(FLASH_LAYOUT_AS7265X_FW_START) - 1)
#define FLASH_LAYOUT_AS7265X_FW_SIZE        (262144) /* bytes */


#endif /* FLASH_LAYOUT_H_INCLUDED */
