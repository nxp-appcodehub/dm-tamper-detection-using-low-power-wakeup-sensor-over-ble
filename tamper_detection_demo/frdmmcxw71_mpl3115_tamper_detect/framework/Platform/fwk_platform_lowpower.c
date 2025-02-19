/* -------------------------------------------------------------------------- */
/*                           Copyright 2021-2023 NXP                          */
/*                    SPDX-License-Identifier: BSD-3-Clause                   */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include <stdbool.h>

#include "fwk_platform.h"
#include "fwk_platform_lowpower.h"
#include "fwk_platform_ics.h"
#include "fwk_debug.h"

/* SDK drivers */
#include "fsl_spc.h"
#include "fsl_wuu.h"
#include "fsl_cmc.h"

/* SDK components */
#include "fsl_pm_core.h"
#include "fsl_component_mem_manager.h"

/* FWK services */
#include "FunctionLib.h"

/* -------------------------------------------------------------------------- */
/*                               Private macros                               */
/* -------------------------------------------------------------------------- */

#define PWR_CTCM0_START_ADDR (0x04000000U)
#define PWR_CTCM0_END_ADDR   (0x04001FFFU)
#define PWR_CTCM1_START_ADDR (0x04002000U)
#define PWR_CTCM1_END_ADDR   (0x04003FFFU)
#define PWR_STCM0_START_ADDR (0x20000000U)
#define PWR_STCM0_END_ADDR   (0x20003FFFU)
#define PWR_STCM1_START_ADDR (0x20004000U)
#define PWR_STCM1_END_ADDR   (0x20007FFFU)
#define PWR_STCM2_START_ADDR (0x20008000U)
#define PWR_STCM2_END_ADDR   (0x2000FFFFU)
#define PWR_STCM3_START_ADDR (0x20010000U)
#define PWR_STCM3_END_ADDR   (0x20017FFFU)
#define PWR_STCM4_START_ADDR (0x20018000U)
#define PWR_STCM4_END_ADDR   (0x20019FFFU)
#define PWR_STCM5_START_ADDR (0x2001A000U)
#define PWR_STCM5_END_ADDR   (0x2001BFFFU)

#define PWR_CTCM0_IDX 0U
#define PWR_CTCM1_IDX 1U
#define PWR_STCM0_IDX 2U
#define PWR_STCM1_IDX 3U
#define PWR_STCM2_IDX 4U
#define PWR_STCM3_IDX 5U
#define PWR_STCM4_IDX 6U
#define PWR_STCM5_IDX 7U

#if defined(gPlatformShutdownEccRamInLowPower) && (gPlatformShutdownEccRamInLowPower > 0)
/* In this configuration, all RAM banks can be shutdown during low power if not used
 * The ECC RAM banks can be selectively reinitialized with MEM_ReinitRamBank API
 * This API is also used by the Memory Manager Light */
#define PWR_SELECT_RAM_RET_START_IDX 0U
#define PWR_SELECT_RAM_RET_END_IDX   7U
#else
/* STCM3 and STCM4 only are non-ECC RAM banks */
#define PWR_SELECT_RAM_RET_START_IDX 5U
#define PWR_SELECT_RAM_RET_END_IDX   6U
#endif /* gPlatformShutdownEccRamInLowPower */

#define SPC_VD_STAT_CLEAR_MASK                                                                    \
    (SPC_VD_STAT_COREVDD_LVDF_MASK | SPC_VD_STAT_SYSVDD_LVDF_MASK | SPC_VD_STAT_IOVDD_LVDF_MASK | \
     SPC_VD_STAT_COREVDD_HVDF_MASK | SPC_VD_STAT_SYSVDD_HVDF_MASK | SPC_VD_STAT_IOVDD_HVDF_MASK)

#define PLATFORM_LPWKUP_DELAY_10MHz (0xAAU)

#define PLATFORM_HOST_USE_POWER_DOWN (0xA5A5A5A5U)

/* -------------------------------------------------------------------------- */
/*                             Private prototypes                             */
/* -------------------------------------------------------------------------- */

/*!
 * \brief Initializes wake up sources
 *
 */
static void PLATFORM_InitWakeUpSources(void);

/*!
 * \brief Callback registered to SDK Power Manager to get notified of entry/exit of low power modes
 *
 * \param[in] eventType event specifying if we entered or exited from low power mode
 * \param[in] powerState low power mode used during low power period
 * \param[in] data Optional data passed when the callback got registered (not used currently)
 * \return status_t
 */
static status_t PLATFORM_LowPowerCallback(pm_event_type_t eventType, uint8_t powerState, void *data);

/*!
 * \brief Shutdown Radio domain
 *        To be used when the current application is ready to go to RAMOFF
 *
 */
static void PLATFORM_ShutdownRadio(void);

/*!
 * \brief Set low power constraint to the SDK PowerManager
 *
 * \param[in] powerMode power mode ID
 *                      0: WFI
 *                      1: SLEEP
 *                      2: DEEP SLEEP
 *                      3: POWER DOWN
 *                      4: DEEP POWER DOWN
 * \return int32_t int 0 if success, negative if error.
 */
static int32_t PLATFORM_SetLowPowerConstraint(int32_t powerMode);

/*!
 * \brief Release low power constraint to the SDK PowerManager
 *
 * \param[in] powerMode power mode ID
 *                      0: WFI
 *                      1: SLEEP
 *                      2: DEEP SLEEP
 *                      3: POWER DOWN
 *                      4: DEEP POWER DOWN
 * \return int32_t int 0 if success, negative if error.
 */
static int32_t PLATFORM_ReleaseLowPowerConstraint(int32_t powerMode);

void WUU0_IRQHandler(void);

/* -------------------------------------------------------------------------- */
/*                             Private variables                              */
/* -------------------------------------------------------------------------- */

static pm_notify_element_t platformLpNotifyGroup = {
    .notifyCallback = PLATFORM_LowPowerCallback,
    .data           = NULL,
};

static const uint32_t TCM_START_ADDR_tab[] = {
    PWR_CTCM0_START_ADDR, PWR_CTCM1_START_ADDR, PWR_STCM0_START_ADDR, PWR_STCM1_START_ADDR,
    PWR_STCM2_START_ADDR, PWR_STCM3_START_ADDR, PWR_STCM4_START_ADDR, PWR_STCM5_START_ADDR,
};

static const uint32_t TCM_END_ADDR_tab[] = {
    PWR_CTCM0_END_ADDR, PWR_CTCM1_END_ADDR, PWR_STCM0_END_ADDR, PWR_STCM1_END_ADDR,
    PWR_STCM2_END_ADDR, PWR_STCM3_END_ADDR, PWR_STCM4_END_ADDR, PWR_STCM5_END_ADDR,
};

#if defined(gPlatformShutdownEccRamInLowPower) && (gPlatformShutdownEccRamInLowPower > 0)
static const bool TCM_IS_ECC[] = {
    TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, TRUE,
};

static uint32_t last_bank_mask = 0xFFU;
#endif /* gPlatformShutdownEccRamInLowPower */

static const spc_power_domain_id_t pd_name_2_id[2U] = {
    [PLATFORM_MainDomain]   = kSPC_PowerDomain0, /*!<  Main Power domain containing the main processor        */
    [PLATFORM_WakeupDomain] = kSPC_PowerDomain1, /*!<  Wake up Power domain containing the wake up peripheral */
};

static const PLATFORM_PowerDomainState_t spc_lp_name_2_pd_state[9U] = {
    [kSPC_SleepWithSYSClockRunning] = PLATFORM_NO_LOWPOWER, /*!<   Power domain has not been in Low power mode       */
    [kSPC_SleepWithSysClockOff]     = PLATFORM_SLEEP_MODE,  /*!<   Power domain has not been in Sleep mode           */
    [kSPC_DeepSleepSysClockOff] = PLATFORM_DEEP_SLEEP_MODE, /*!<   Power domain has not been in Deep Sleep mode      */
    [kSPC_PowerDownWithSysClockOff] = PLATFORM_POWER_DOWN_MODE, /*!<   Power domain has not been in Power Down mode */
    [kSPC_DeepPowerDownWithSysClockOff] =
        PLATFORM_DEEP_POWER_DOWN_MODE, /*!<   Power domain has not been in Deep Power Down mode */
};

static const FwkSrv_LowPowerConstraintCallbacks_t fwkSrvLowPowerConstraintCallbacks = {
    .fwkSrvSetLowPowerConstraint     = PLATFORM_SetLowPowerConstraint,
    .fwkSrvReleaseLowPowerConstraint = PLATFORM_ReleaseLowPowerConstraint,
};

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

void PLATFORM_LowPowerInit(void)
{
    status_t status;

    /* Register the low power Notify callback as high priority (kPM_NotifyGroup2) */
    status = PM_RegisterNotify(kPM_NotifyGroup2, &platformLpNotifyGroup);
    assert(status == kStatus_Success);
    (void)status;

    /* Initialize required wake up sources */
    PLATFORM_InitWakeUpSources();

    /* Clear voltage detect status */
    SPC_ClearVoltageDetectStatusFlag(SPC0, SPC_VD_STAT_CLEAR_MASK);

    /* Set number of SPC timer clock cycles to wait on exit from lowpower mode */
    SPC_SetLowPowerWakeUpDelay(SPC0, PLATFORM_LPWKUP_DELAY_10MHz);

    /* When low power is enabled on the host core, the radio core may need to set/release low power constraints
     * as some resources needed by it are in the host power domain
     * This needs to be done before starting the radio core to avoid any race condition */
    PLATFORM_FwkSrvRegisterLowPowerCallbacks(&fwkSrvLowPowerConstraintCallbacks);
}

void PLATFORM_EnterLowPower(void)
{
#if defined(KW45_A0_SUPPORT) && (KW45_A0_SUPPORT > 0)
    /* Disable ACTIVE_CFG LVD to avoid reset while going to low power
     * when Core LDO voltage is higher than 1.0V (KFOURWONE-1703) */
    (void)SPC_EnableActiveModeCoreLowVoltageDetect(SPC0, false);
#endif
    return;
}

void PLATFORM_ExitLowPower(void)
{
#if defined(KW45_A0_SUPPORT) && (KW45_A0_SUPPORT > 0)
    /* Re-enable ACTIVE_CFG LVD when exiting low power (KFOURWONE-1703) */
    (void)SPC_EnableActiveModeCoreLowVoltageDetect(SPC0, true);
#endif
    return;
}

void PLATFORM_EnterPowerDown(void)
{
    /* Warning : For power down, it requires gLowpowerPowerDownEnable_d variable to be set to 1 in linker file.
     *    Generate assertion if not the case */
    extern uint32_t gLowpowerPowerDownEnable_d[];
    assert((uint32_t)&gLowpowerPowerDownEnable_d == 1);
    (void)gLowpowerPowerDownEnable_d;

    /* Transmit a message to the nbu via the shared memory to warn it that the host go in power down */
    static bool_t power_down_on_host_message_sent = false;
    if (!power_down_on_host_message_sent)
    {
        extern uint32_t m_lowpower_flag_start[];
        PLATFORM_RemoteActiveReq();
        *(volatile unsigned int *)(m_lowpower_flag_start) = (uint32_t)PLATFORM_HOST_USE_POWER_DOWN;
        PLATFORM_RemoteActiveRel();
        power_down_on_host_message_sent = true;
    }

    return;
}

void PLATFORM_ExitPowerDown(void)
{
    return;
}

void PLATFORM_EnterDeepPowerDown(void)
{
    /* Put NBU domain in Deep Power Down */
    PLATFORM_ShutdownRadio();
}

void WUU0_IRQHandler(void)
{
    status_t status;

    // PWR_DBG_LOG("");

    /* The Power Manager will parse all the registered and enabled wake up sources
     * and determine if they triggered the WUU IRQ. If so, it will call each
     * wake up source's callback */
    status = PM_HandleWakeUpEvent();
    assert(status == kStatus_PMSuccess);
    (void)status;

    /* Clear WUU pin status flag */
    WUU_ClearExternalWakeUpPinsFlag(WUU0, WUU_GetExternalWakeUpPinsFlag(WUU0));
}

PLATFORM_status_t PLATFORM_GetLowpowerMode(PLATFORM_PowerDomain_t power_domain, PLATFORM_PowerDomainState_t *pd_state)
{
    spc_power_domain_id_t pd_id  = pd_name_2_id[power_domain];
    PLATFORM_status_t     status = PLATFORM_Successful;

    if (SPC_CheckPowerDomainLowPowerRequest(SPC0, pd_id) == true)
    {
        spc_power_domain_low_power_mode_t spcLowPowermode;
        spcLowPowermode = SPC_GetPowerDomainLowPowerMode(SPC0, pd_id);

        if ((uint32_t)spcLowPowermode >= sizeof(spc_lp_name_2_pd_state))
        {
            status = PLATFORM_Error;
        }
        else
        {
            *pd_state = spc_lp_name_2_pd_state[spcLowPowermode];
        }
    }
    else
    {
        *pd_state = PLATFORM_NO_LOWPOWER;
    }

    return status;
}

uint8_t PLATFORM_GetDefaultRamBanksRetained(void)
{
    uint32_t          ram_upper_limit;
    uint32_t          ram_lower_limit;
    uint8_t           bank_mask = 0xFFU; // Retain everything by default
    volatile uint32_t heap_end;

    /* Those symbols should be exported by the Linker Script */
    extern uint32_t firmware_ram_upper_limit[];
    extern uint32_t firmware_ram_lower_limit[];
    extern uint32_t __HEAP_end__[];

    /* Some compilers can try to optimize the comparison between two external addresses because they will think that
     * they come from two global values and therefore that it is impossible for them to have the same address, but in
     * our case these values come from our linker. That's why we used a local volatile value to make sure it won't be
     * optimized */
    heap_end = (uint32_t)&__HEAP_end__;

    /* Lower ram limit is not necessarily in the first RAM bank
     * On KW45, the 2 first banks are Code TCM banks, but usually not used */
    ram_lower_limit = (uint32_t)&firmware_ram_lower_limit;

    /* Optimized linker script and memory manager, the heap is placed on the top of the RAM */
    if ((uint32_t)&firmware_ram_upper_limit == heap_end)
    {
        ram_upper_limit = MEM_GetHeapUpperLimit();
    }
    else
    {
        ram_upper_limit = (uint32_t)&firmware_ram_upper_limit;
    }

    assert(ram_upper_limit > ram_lower_limit);

    /* Go through each selectables RAM banks and check if we can shutdown some
     * of them */
    for (uint8_t i = PWR_SELECT_RAM_RET_START_IDX; i <= PWR_SELECT_RAM_RET_END_IDX; i++)
    {
        if ((TCM_START_ADDR_tab[i] >= ram_upper_limit) || (TCM_END_ADDR_tab[i] <= ram_lower_limit))
        {
            /* This RAM bank is outside current used RAM range, so we can
             * shut it down during low power */
            bank_mask &= ~(1U << i);
        }
    }

    return bank_mask;
}

void PLATFORM_SetRamBanksRetained(uint8_t bank_mask)
{
    /* Set SRAM retention config handled by the CMC module */
    CMC_PowerOffSRAMLowPowerOnly(CMC0, ~bank_mask);

    /* STCM5 is handled by VBAT module */
    uint32_t vbat_ldoramc = VBAT0->LDORAMC;
    vbat_ldoramc &= ~VBAT_LDORAMC_RET_MASK;
    vbat_ldoramc |= VBAT_LDORAMC_RET((uint32_t)(~((uint32_t)bank_mask & ((uint32_t)1U << PWR_STCM5_IDX)) >> 7));
    VBAT0->LDORAMC = vbat_ldoramc;

#if defined(gPlatformShutdownEccRamInLowPower) && (gPlatformShutdownEccRamInLowPower > 0)
    last_bank_mask = bank_mask;
#endif
}

/* -------------------------------------------------------------------------- */
/*                              Private functions                             */
/* -------------------------------------------------------------------------- */

/*!
 * \brief Initializes wake up sources
 *
 */
static void PLATFORM_InitWakeUpSources(void)
{
    /* Enable low power timers (LPTMR) as wake up source */
    WUU_SetInternalWakeUpModulesConfig(WUU0, 0U, kWUU_InternalModuleInterrupt);

    /* Enable Radio Async Wakeup request
     * This is needed to correctly wake up SOC XBAR bus when CM3 needs it */
    WUU_SetInternalWakeUpModulesConfig(WUU0, 2U, kWUU_InternalModuleDMATrigger);

    /* WUU Irq interrupt is required for proper wakeup functionality from lowpower */
    NVIC_EnableIRQ(WUU0_IRQn);
}

static status_t PLATFORM_LowPowerCallback(pm_event_type_t eventType, uint8_t powerState, void *data)
{
    (void)data;

#if defined(gDbg_Enabled_d) && (gDbg_Enabled_d > 0)
    static int lp_nb = 0;
    if (eventType == kPM_EventEnteringSleep)
    {
        PWR_DBG_LOG("[%d] evt=%d pwrstate=%d", lp_nb, eventType, powerState);
    }
#endif
    /* Nothing to do on Sleep state (WFI) but only Deeper low power mode */
    if (powerState < PLATFORM_DEEP_SLEEP_STATE)
    {
        /* Nothing to do when entering WFI or Sleep low power state
            NVIC fully functionnal to trigger upcoming interrupts */
    }
    else
    {
        if (eventType == kPM_EventEnteringSleep)
        {
#if defined(gDbg_Enabled_d) && (gDbg_Enabled_d > 0)
            /* NXP logging dump */
#define DUMP_EVERY_X_TIMES 4
            static int count = DUMP_EVERY_X_TIMES;
            if (--count == 0)
            {
                DBG_LOG_DUMP();
                count = DUMP_EVERY_X_TIMES;
            }
#endif
            /* Platform module can implement platform specific methods to execute
             * when entering and exiting any low power mode.
             * Those methods should implement only mandatory procedures for the
             * platform, compatible with any connectivity protocol */
#if 0
            uint8_t mode;
            mode = PM_GetAllowedLowestPowerMode();
            if (mode  == (PLATFORM_DEEP_POWER_DOWN_STATE))
            {
                BOARD_DbgStopLoggingTimer();
            }
#endif

            PLATFORM_EnterLowPower();

            if (powerState >= PLATFORM_POWER_DOWN_STATE)
            {
                /* Power gated low power modes often require extra specific
                 * entry/exit low power procedures, those should be implemented
                 * in the following PLATFORM API */
                PLATFORM_EnterPowerDown();
            }

            if (powerState == PLATFORM_DEEP_POWER_DOWN_STATE)
            {
                /* Perform specific procedures when entering RAMOFF such as
                 * powering off the radio domain */
                PLATFORM_EnterDeepPowerDown();
            }
        }
        else
        {
            /* Check if Main power domain domain really went to Power down,
             *   powerState variable is just an indication, Lowpower mode could have been skipped by an immediate wakeup
             */
            PLATFORM_PowerDomainState_t main_pd_state = PLATFORM_NO_LOWPOWER;
            PLATFORM_status_t           status;
            status = PLATFORM_GetLowpowerMode(PLATFORM_MainDomain, &main_pd_state);
            assert(status == PLATFORM_Successful);
            (void)status;
#if 0
            PLATFORM_PowerDomainState_t   wakeup_pd_state;
            PLATFORM_GetLowpowerMode(PLATFORM_WakeupDomain, &wakeup_pd_state);
            PWR_DBG_LOG("main_pd=%d wakeup_pd=%d", main_pd_state, wakeup_pd_state);
#endif

            if (main_pd_state == PLATFORM_POWER_DOWN_MODE)
            {
                /* Power gated low power modes often require specific
                 * entry/exit low power procedures, those should be implemented
                 * in the following PLATFORM API */
                PLATFORM_ExitPowerDown();
            }

            // DBG_LOG_WAKEUP_CHECK();

            /* Platform specific procedures to execute when exiting low power mode
             * any low power mode */
            PLATFORM_ExitLowPower();
        }
    }

    /* Debug Only */
#if defined(gDbg_Enabled_d) && (gDbg_Enabled_d > 0)
    /* On wakeup, display pending interrupts */
    if (eventType != kPM_EventEnteringSleep)
    {
        lp_nb++;
        BOARD_DbgCheckIrqPending(false);
    }
#endif

    return kStatus_Success;
}

static void PLATFORM_ShutdownRadio(void)
{
    /* Reset all RFMC registers and put the NBU CM3 in reset */
    RFMC->CTRL |= RFMC_CTRL_RFMC_RST(0x1U);
    /* Wait for a few microseconds before releasing the NBU reset,
     * without this the system may hang in the loop waiting for FRO clock valid */
    PLATFORM_Delay(31u);
    /* Release NBU reset */
    RFMC->CTRL &= ~RFMC_CTRL_RFMC_RST_MASK;

    /* NBU was probably in low power before the RFMC reset, so we need to wait for the FRO clock to be valid before
     * accessing RF_CMC */
    while ((RFMC->RF2P4GHZ_STAT & RFMC_RF2P4GHZ_STAT_FRO_CLK_VLD_STAT_MASK) == 0U)
    {
        ;
    }

    /* Configure the RF_CMC1 where the NBU, FRO and flash are not used */
    RF_CMC1->RADIO_LP |= RF_CMC1_RADIO_LP_CK(0x2);

    /* Force low power entry request to the radio domain */
    RFMC->RF2P4GHZ_CTRL |= RFMC_RF2P4GHZ_CTRL_LP_ENTER(0x1U);
}

#if defined(gPlatformShutdownEccRamInLowPower) && (gPlatformShutdownEccRamInLowPower > 0)
void MEM_ReinitRamBank(uint32_t startAddress, uint32_t endAddress)
{
    bool     indexFound = false;
    uint32_t startIndex = 0xFFU;
    uint32_t endIndex   = 0xFFU;

    /* This for loop will find the RAM banks which cover the requested address range.
     * The range could be large enough to contain more than one bank */
    for (uint32_t i = PWR_SELECT_RAM_RET_START_IDX; i <= PWR_SELECT_RAM_RET_END_IDX; i++)
    {
        if ((startAddress >= TCM_START_ADDR_tab[i]) && (startAddress <= TCM_END_ADDR_tab[i]))
        {
            /* The start address is contained in this bank */
            startIndex = i;
        }

        if ((endAddress >= TCM_START_ADDR_tab[i]) && (endAddress <= TCM_END_ADDR_tab[i]))
        {
            /* The end address is contained in this bank */
            endIndex = i;
        }

        if ((startIndex != 0xFFU) && (endIndex != 0xFFU))
        {
            /* Both indexes have be found, we can proceed further */
            indexFound = true;
            break;
        }
    }

    if (indexFound == true)
    {
        /* This loop will go through the different RAM banks based on the indexes found
         * right before. If the current bank is an ECC bank and if it has been shutdown
         * during last low power period, then we proceed with the memset to force ECC
         * calculation */
        for (uint32_t i = startIndex; i <= endIndex; i++)
        {
            if (TCM_IS_ECC[i] == true)
            {
                if ((last_bank_mask & ((uint32_t)1U << i)) == 0U)
                {
                    uint32_t size, bankAdressToReinit;

                    size = TCM_END_ADDR_tab[i] - TCM_START_ADDR_tab[i] + 1U;

                    bankAdressToReinit = TCM_START_ADDR_tab[i];

                    /* Write to ECC RAM bank to force ECC calculation
                        Warning : This is mandatory to write word per word (not by byte per byte),  so use a proprietary
                        FuncLib API to make sure the memory set is done by word */
                    FLib_MemSet32Aligned((uint32_t *)bankAdressToReinit, 0U, (size / 4U));

                    /* Remove this bank from the last shutdown during low power as
                     * it has been reinitialized */
                    last_bank_mask |= ((uint32_t)1U << i);
                }
            }
        }
    }
}
#endif /* gPlatformShutdownEccRamInLowPower */

static int32_t PLATFORM_SetLowPowerConstraint(int32_t powerMode)
{
    int32_t  ret = 0;
    status_t st  = kStatus_Success;

    switch (powerMode)
    {
        case 0:
            st = PM_SetConstraints(PLATFORM_WFI_STATE, PLATFORM_WFI_CONSTRAINTS);
            break;

        case 1:
            st = PM_SetConstraints(PLATFORM_SLEEP_STATE, PLATFORM_SLEEP_CONSTRAINTS);
            break;

        case 2:
            st = PM_SetConstraints(PLATFORM_DEEP_SLEEP_STATE, PLATFORM_DEEP_SLEEP_CONSTRAINTS);
            break;

        case 3:
            st = PM_SetConstraints(PLATFORM_POWER_DOWN_STATE, PLATFORM_POWER_DOWN_CONSTRAINTS);
            break;

        case 4:
            st = PM_SetConstraints(PLATFORM_DEEP_POWER_DOWN_STATE, PLATFORM_DEEP_POWER_DOWN_CONSTRAINTS);
            break;

        default:
            ret = -1;
            break;
    }

    if (st != kStatus_Success)
    {
        ret = -1;
    }

    return ret;
}

static int32_t PLATFORM_ReleaseLowPowerConstraint(int32_t powerMode)
{
    int32_t  ret = 0;
    status_t st  = kStatus_Success;

    switch (powerMode)
    {
        case 0:
            st = PM_ReleaseConstraints(PLATFORM_WFI_STATE, PLATFORM_WFI_CONSTRAINTS);
            break;

        case 1:
            st = PM_ReleaseConstraints(PLATFORM_SLEEP_STATE, PLATFORM_SLEEP_CONSTRAINTS);
            break;

        case 2:
            st = PM_ReleaseConstraints(PLATFORM_DEEP_SLEEP_STATE, PLATFORM_DEEP_SLEEP_CONSTRAINTS);
            break;

        case 3:
            st = PM_ReleaseConstraints(PLATFORM_POWER_DOWN_STATE, PLATFORM_POWER_DOWN_CONSTRAINTS);
            break;

        case 4:
            st = PM_ReleaseConstraints(PLATFORM_DEEP_POWER_DOWN_STATE, PLATFORM_DEEP_POWER_DOWN_CONSTRAINTS);
            break;

        default:
            ret = -1;
            break;
    }

    if (st != kStatus_Success)
    {
        ret = -1;
    }

    return ret;
}
