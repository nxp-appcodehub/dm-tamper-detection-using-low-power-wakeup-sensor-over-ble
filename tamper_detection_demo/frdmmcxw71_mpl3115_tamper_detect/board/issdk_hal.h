/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! \file issdk_hal.h
    \brief Wrapper for Hardware Abstraction Layer (HAL)

    This file simply provides one level of indirection for the developer
    to select the particular Hardware Abstraction Layer they would like to use.
*/

#ifndef __ISSDK_HAL_H__
#define __ISSDK_HAL_H__

#include "fsl_lpi2c_cmsis.h"
#include "fsl_lpspi_cmsis.h"
#include "fsl_lpuart_cmsis.h"

#include "frdmmcxw7x.h"               //Include appropriate MCU board header file
#include "frdm_stbc_p3115_shield.h" //Include appropriate sensor shield board header file

// Pin mapping and driver information for default I2C brought to shield
// By default, we use I2C_S1 defined in the frdmmcxw7x.h file.
// Other options: I2C_S2.
// S1 is on A5:4.  S2 is on D15:14.
#define I2C_S_DRIVER       I2C_S2_DRIVER
#define I2C_S_SIGNAL_EVENT I2C_S2_SIGNAL_EVENT
#define I2C_S_DEVICE_INDEX I2C_S2_DEVICE_INDEX

#endif // __ISSDK_HAL_H__
