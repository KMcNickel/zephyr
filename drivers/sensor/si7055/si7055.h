/*
 * Copyright (c) 2020 Laird Connectivity
 * Copyright (c) 2019 Electronut Labs
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _SI7055_H
#define _SI7055_H

/* Si7055 register addresses */
#define SI7055_MEAS_TEMP_MASTER_MODE                    0xE3
#define SI7055_MEAS_TEMP_NO_MASTER_MODE                 0xF3
#define SI7055_RESET                                    0xFE
#define SI7055_READ_ID_LOW_0                            0xFA
#define SI7055_READ_ID_LOW_1                            0x0F
#define SI7055_READ_ID_HIGH_0                           0xFC
#define SI7055_READ_ID_HIGH_1                           0xC9
#define SI7055_FIRMWARE_0                               0x84
#define SI7055_FIRMWARE_1                               0xB8

#endif /* _SI7055_H */
