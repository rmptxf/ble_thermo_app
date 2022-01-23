/* ****************************************************************************
 * Copyright (C) Bojan Potočnik - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bojan Potočnik <info@bojanpotocnik.com>, 2020
 *************************************************************************** */

#ifndef VERSION_H_INCLUDED
#define VERSION_H_INCLUDED


/* Hardware version number is only checked in DFU bootloader to match exactly,
 * therefore increasing number is not important. As currently string is used
 * as a hardware version descriptor, the following Python code is used to
 * calculate integer version of the HW version (yes, it is true that hash()
 * function uses random seed, however the VERSION_HW_INT is hardcoded in the
 * update FW binary):
 * > VERSION_HW = "TemplateHW v1.0"
 * > hash(VERSION_HW) % 0x7FFFFFFF
 */
#define VERSION_HW          "JJ04112020LJ-1"
#define VERSION_HW_INT      1295364420ul


static const unsigned char VERSION_FW_MAJOR = 0;
static const unsigned char VERSION_FW_MINOR = 1;
static const unsigned char VERSION_FW_BUILD = 0;
/* Application/Firmware version is checked in the DFU bootloader to prevent
 * downgrade of the application version. Therefore it must match predefined
 * algorithm which is also used when generating DFU .zip file on the PC.
 * Base 10 multiply is used instead of << 8 so it is easier to read and it
 * fits in 32-bit number in any case.
 */
static const unsigned int VERSION_FW_INT = 1000000ul * VERSION_FW_MAJOR + 1000ul * VERSION_FW_MINOR + VERSION_FW_BUILD;


/** Compiler build time in format "HH:MM:SS" (defined in main.c) */
extern const char VERSION_FW_BUILD_TIME[sizeof(__TIME__)];

/** Compiler build date in format "Dec 31 2020" (defined in main.c) */
extern const char VERSION_FW_BUILD_DATE[sizeof(__DATE__)];


#endif /* VERSION_H_INCLUDED */
