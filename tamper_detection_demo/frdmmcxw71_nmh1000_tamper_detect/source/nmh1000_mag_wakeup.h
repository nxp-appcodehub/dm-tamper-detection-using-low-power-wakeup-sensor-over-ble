/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file nmh1000_mag_wakeup.h
 * @brief The nmh1000_mag_wakeup.h file details NMH1000 magnetic switch sensor
 *        configuration to detect magnetic tampering and raise an alert.
 */
#ifndef NMH1000_MAG_WAKEUP_H_
#define NMH1000_MAG_WAKEUP_H_

//-----------------------------------------------------------------------
// ISSDK Includes
//-----------------------------------------------------------------------
#include <nmh1000_mag_wakeup.h>
#include "issdk_hal.h"
#include "gpio_driver.h"
#include "nmh1000_drv.h"
#include "systick_utils.h"

//-----------------------------------------------------------------------
// CMSIS Includes
//-----------------------------------------------------------------------
#include "Driver_I2C.h"

//-----------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------
#define NMH1000_DATA_SIZE (1) /* 1 byte Mag Data. */
#define NMH1000_NUM_REGISTERS (NMH1000_I2C_ADDR + 1)
#define THRESHOLD 50
//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------
/*! @brief Register settings for configuring SDCD-OT for tap-detection and Auto-Wake/Sleep in interrupt mode. */

/*! @brief Register settings for Normal Mode. */
const registerwritelist_t cNmh1000ConfigNormal[] = {
    {NMH1000_ODR, NMH1000_USER_ODR_ODR_10X_HSP, NMH1000_USER_ODR_ODR_MASK},
    {NMH1000_CONTROL_REG1, NMH1000_CONTROL_REG1_AUTO_MODE_START, NMH1000_CONTROL_REG1_AUTO_MODE_MASK},
    __END_WRITE_DATA__};

/*! @brief Address and size of Raw Pressure+Temperature Data in Normal Mode. */
const registerreadlist_t cNmh1000OutputNormal[] = {{.readFrom = NMH1000_OUT_M_REG, .numBytes = NMH1000_DATA_SIZE},
                                                 __END_READ_DATA__};


//-----------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------

    ARM_DRIVER_I2C *I2Cdrv = &I2C_S_DRIVER;
    nmh1000_i2c_sensorhandle_t nmh1000Driver;
    /* Define the init structure for the output LED pin*/
    gpio_pin_config_t led_config = {
        kGPIO_DigitalOutput,
        0,
    };

//-----------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------






#endif /* NMH1000_MAG_WAKEUP_H_ */
