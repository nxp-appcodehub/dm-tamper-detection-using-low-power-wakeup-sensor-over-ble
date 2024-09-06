/* -------------------------------------------------------------------------- */
/*                           Copyright 2021-2023 NXP                          */
/*                            All rights reserved.                            */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

#ifndef _FWK_PLATFORM_SENSORS_H_
#define _FWK_PLATFORM_SENSORS_H_

/*!
 * @addtogroup FWK_Platform_module
 * @{
 */
/*!
 * @addtogroup FWK_Platform_Sensors
 * The FWK_Platform_Sensors module
 *
 * FWK_Platform_Sensors module provides APIs for temperature and battery measurement.
 *
 * \details FWK_Platform_Sensors API are used by Sensors module.
 * @{
 */

#include <stdbool.h>

/*!
 * Value used to indicated that temperature is unknown
 */
#define PLATFORM_SENSOR_UNKNOWN_TEMPERATURE ((int32_t)999999)

/* -------------------------------------------------------------------------- */
/*                        Public functions declaration                        */
/* -------------------------------------------------------------------------- */

/*!
 * \brief Check if the ADC driver is initialized.
 *
 * \retval bool Is the ADC initialized or not.
 *
 */
bool PLATFORM_IsAdcInitialized(void);

/*!
 * \brief Init ADC driver.
 *
 */
void PLATFORM_InitAdc(void);

/*!
 * \brief Reset ADC init flag.
 *
 */
void PLATFORM_DeinitAdc(void);

/*!
 * \brief Trig the ADC on the battery.
 *
 */
void PLATFORM_StartBatteryMonitor(void);

/*!
 * \brief Get battery level from the ADC in percentage.
 *
 * \details 3V or more = 100%
 *        1.8V or less = 0%
 *
 * \param  [out] battery_level           The battery level is returned by reference
 */
void PLATFORM_GetBatteryLevel(uint8_t *battery_level);

/*!
 * \brief Trig the ADC on the temperature.
 *
 */
void PLATFORM_StartTemperatureMonitor(void);

/*!
 * \brief Get temperature value from the ADC, if the value change significantly the
 *        result is send to the CM3.
 *
 * \param  [out] temperature_value       The temperature value is returned by reference in tenths of degree celsius
 */
void PLATFORM_GetTemperatureValue(int32_t *temperature_value);

/*!
 * \brief Save ADC Context
 *
 */
void PLATFORM_SaveAdcContext(void);

/*!
 * \brief Restore ADC context
 *
 */
void PLATFORM_RestoreAdcContext(void);

/*!
 * @}  end of FWK_Platform_Sensors addtogroup
 */
/*!
 * @}  end of FWK_Platform_module addtogroup
 */
#endif /* _FWK_PLATFORM_SENSORS_H_ */
