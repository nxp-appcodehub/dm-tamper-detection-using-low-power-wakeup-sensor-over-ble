/*! *********************************************************************************
* \addtogroup HCI
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright 2014 Freescale Semiconductor, Inc.
* Copyright 2016-2024 NXP
*
*
* \file
*
* This file is the interface file for the HCI module
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef HCI_TYPES_H
#define HCI_TYPES_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "ble_general.h"

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/
#if defined(__CC_ARM)
  #pragma anon_unions
#endif

/* Generic constants */
#define gHciLeAdvertisingDataLength_c       (31U)
#define gHciLeScanResponseDataLength_c      (31U)

#if defined(gBLE50_d) && (gBLE50_d == TRUE)
#define gHciNumLeMetaSubEvents_c            (21U)
#else
#define gHciNumLeMetaSubEvents_c            (11U)
#endif
#define gHciNumEvents_c                     (9U)

#if defined(gBLE50_d) && (gBLE50_d == TRUE)
#define gHciLeExtAdvertisingDataLength_c    (251U)
#define gHciLeExtScanRespDataLength_c       (251U)
#define gHciMaxPHYs_c                       (3U)     /* 1M + 2M + Coded */
#define gHciMaxScanningPHYs_c               (2U)     /* 1M + Coded */
#define gHciMaxAdvertisingSets_c            (63U)
#if defined(gBLE50_PeriodicAdvSupport_d) && (gBLE50_PeriodicAdvSupport_d == TRUE)
#define gHciMaxPeriodicAdvDataLength_c      (252U)
#endif /* defined(gBLE50_PeriodicAdvSupport_d) && (gBLE50_PeriodicAdvSupport_d == TRUE) */
#define gHciMaxAdvReportDataLen_c           (229U)
#if defined(gBLE50_PeriodicAdvSupport_d) && (gBLE50_PeriodicAdvSupport_d == TRUE)
#define gHciMaxPeriodicAdvReportDataLen_c   (247U) /* ESR11: E8909 - Wrong Maximum */
#endif /* defined(gBLE50_PeriodicAdvSupport_d) && (gBLE50_PeriodicAdvSupport_d == TRUE) */
#endif

#if defined(gBLE51_d) && (gBLE51_d == TRUE)
#define gHciMaxSwitchingPatternLength_c     (75U)
#endif

/* HCI packets header lengths */
#define gHciCommandPacketHeaderLength_c     (3U)
#define gHciAclDataPacketHeaderLength_c     (4U)
#define gHciEventPacketHeaderLength_c       (2U)

/* Both the Host and the Controller shall support command and event packets, where
the data portion (excluding header) contained in the packets is 500 - header size octets in size. */
#define gHcLeAclDataPacketLengthDefault_c   (500U - gHciAclDataPacketHeaderLength_c)
#define gHcEventPacketLengthDefault_c       (500U - gHciEventPacketHeaderLength_c)

    /*! HCI OGF : 0x01 : Link Control Commands */
#define gHciLinkControlCommands_c                       0x01U
        /*! HCI Commands OCF for OGF : 0x01 : Link Control Commands */
#define gHciDisconnect_c                          0x0006U
#define gHciReadRemoteVersionInformation_c        0x001DU


    /*! HCI OGF : 0x03 : Controller & Baseband Commands */
#define gHciControllerBasebandCommands_c                0x03U
        /*! HCI Commands OCF for OGF : 0x03 : Controller & Baseband Commands */
#define gHciSetEventMask_c                        0x0001U
#define gHciReset_c                               0x0003U
#define gHciReadTransmitPowerLevel_c              0x002DU
#define gHciSetControllerToHostFlowControl_c      0x0031U
#define gHciHostBufferSize_c                      0x0033U
#define gHciHostNumberOfCompletedPackets_c        0x0035U
#define gHciSetEventMaskPage2_c                   0x0063U

#if defined(gLePingIsSupported_d) && (gLePingIsSupported_d == TRUE)
#define gHciReadAuthenticatedPayloadTimeout_c     0x007BU
#define gHciWriteAuthenticatedPayloadTimeout_c    0x007CU
#endif


    /*! HCI OGF : 0x04 : Informational Parameters */
#define gHciInformationalParameters_c                   0x04U
        /*! HCI Commands OCF for OGF : 0x04 : Informational Parameters */
#define gHciReadLocalVersionInformation_c         0x0001U
#define gHciReadLocalSupportedCommands_c          0x0002U
#define gHciReadLocalSupportedFeatures_c          0x0003U
#define gHciReadBufferSize_c                      0x0005U
#define gHciReadBdAddr_c                          0x0009U


    /*! HCI OGF : 0x05 : Status Parameters */
#define gHciStatusParameters_c                          0x05U
        /*! HCI Commands OCF for OGF : 0x05 : Status Parameters */
#define gHciReadRssi_c                           0x0005U

    /*! HCI OGF : 0x08 : LE Controller Commands */
#define gHciLeControllerCommands_c                      0x08U
    /*! HCI Commands OCF for OGF : 0x08 : LE Controller Commands */
#define gHciLeSetEventMask_c                        0x0001U
#define gHciLeReadBufferSize_c                      0x0002U
#define gHciLeReadLocalSupportedFeatures_c          0x0003U
#define gHciLeSetRandomAddress_c                    0x0005U
#define gHciLeSetAdvertisingParameters_c            0x0006U
#define gHciLeReadAdvertisingChannelTxPower_c       0x0007U
#define gHciLeSetAdvertisingData_c                  0x0008U
#define gHciLeSetScanResponseData_c                 0x0009U
#define gHciLeSetAdvertisingEnable_c                0x000AU
#define gHciLeSetScanParameters_c                   0x000BU
#define gHciLeSetScanEnable_c                       0x000CU
#define gHciLeCreateConnection_c                    0x000DU
#define gHciLeCreateConnectionCancel_c              0x000EU
#define gHciLeReadFilterAcceptListSize_c            0x000FU
#define gHciLeClearFilterAcceptList_c               0x0010U
#define gHciLeAddDeviceToFilterAcceptList_c         0x0011U
#define gHciLeRemoveDeviceFromFilterAcceptList_c    0x0012U
#define gHciLeConnectionUpdate_c                    0x0013U
#define gHciLeSetHostChannelClassification_c        0x0014U
#define gHciLeReadChannelMap_c                      0x0015U
#define gHciLeReadRemoteFeatures_c                  0x0016U

#if defined(gHciLeEncryptSupport_d) && (gHciLeEncryptSupport_d == TRUE)
#define gHciLeEncrypt_c                         0x0017U
#endif

#if defined(gHciLeRandomSupport_d) && (gHciLeRandomSupport_d == TRUE)
#define gHciLeRand_c                            0x0018U
#endif

#define gHciLeStartEncryption_c                 0x0019U
#define gHciLeLongTermKeyRequestReply_c         0x001AU
#define gHciLeLongTermKeyRequestNegativeReply_c 0x001BU
#define gHciLeReadSupportedStates_c             0x001CU
#define gHciLeReceiverTest_c                    0x001DU
#define gHciLeTransmitterTest_c                 0x001EU
#define gHciLeTestEnd_c                         0x001FU

#define gHciLeRemoteConnectionParameterRequestReply_c             0x0020U
#define gHciLeRemoteConnectionParameterRequestNegativeReply_c     0x0021U

/* v4.2 specific interface */
#define gHciLeSetDataLength_c                       0x0022U
#define gHciLeReadSuggestedDefaultDataLength_c      0x0023U
#define gHciLeWriteSuggestedDefaultDataLength_c     0x0024U
#define gHciLeReadLocalP256PublicKey_c              0x0025U
#define gHciLeGenerateDhKey_c                       0x0026U
#define gHciLeAddDeviceToResolvingList_c            0x0027U
#define gHciLeRemoveDeviceFromResolvingList_c       0x0028U
#define gHciLeClearResolvingList_c                  0x0029U
#define gHciLeReadResolvingListSize_c               0x002AU
#define gHciLeReadPeerResolvableAddress_c           0x002BU
#define gHciLeReadLocalResolvableAddress_c          0x002CU
#define gHciLeSetAddressResolutionEnable_c          0x002DU
#define gHciLeSetResolvablePrivateAddressTimeout_c  0x002EU
#define gHciLeReadMaximumDataLength_c               0x002FU
#define gHciLeSetPrivacyMode_c                      0x004EU

#define gHciLeEnhancedReceiverTest_c                0x0033U
#define gHciLeEnhancedTransmitterTest_c             0x0034U

/* v5.0 specific interface */
#define gHciLeReadPhy_c                             0x0030U
#define gHciLeSetDefaultPhy_c                       0x0031U
#define gHciLeSetPhy_c                              0x0032U
/* Advertising extensions */
#if defined(gBLE50_d) && (gBLE50_d == TRUE)
#define gHciLeSetAdvSetRandomAddr_c                 0x0035U
#define gHciLeSetExtAdvParams_c                     0x0036U
#define gHciLeSetExtAdvertisingData_c               0x0037U
#define gHciLeSetExtScanRespData_c                  0x0038U
#define gHciLeSetExtAdvEnable_c                     0x0039U
#define gHciLeReadMaxAdvDataLen_c                   0x003AU
#define gHciLeReadNumOfSupportedAdvSets_c           0x003BU
#define gHciLeRemoveAdvSet_c                        0x003CU
#define gHciLeClearAdvSets_c                        0x003DU
#if defined(gBLE50_PeriodicAdvSupport_d) && (gBLE50_PeriodicAdvSupport_d == TRUE)
#define gHciLeSetPeriodicAdvParams_c                0x003EU
#define gHciLeSetPeriodicAdvData_c                  0x003FU
#define gHciLeSetPeriodicAdvEnable_c                0x0040U
#endif /* defined(gBLE50_PeriodicAdvSupport_d) && (gBLE50_PeriodicAdvSupport_d == TRUE) */
#define gHciLeSetExtScanParams_c                    0x0041U
#define gHciLeSetExtScanEnable_c                    0x0042U
#define gHciLeExtCreateConnection_c                 0x0043U
#if defined(gBLE50_PeriodicAdvSupport_d) && (gBLE50_PeriodicAdvSupport_d == TRUE)
#define gHciLePeriodicAdvCreateSync_c               0x0044U
#define gHciLePeriodicAdvCreateSyncCancel_c         0x0045U
#define gHciLePeriodicAdvTerminateSync_c            0x0046U
#define gHciLeAddDeviceToPeriodicAdvList_c          0x0047U
#define gHciLeRemoveDeviceFromPeriodicAdvList_c     0x0048U
#define gHciLeClearPeriodicAdvList_c                0x0049U
#define gHciLeReadPeriodicAdvListSize_c             0x004AU
#endif /* defined(gBLE50_PeriodicAdvSupport_d) && (gBLE50_PeriodicAdvSupport_d == TRUE) */
#define gHciLeReadTransmitPower_c                   0x004BU
#define gHciLeReadRfPathCompensation_c              0x004CU
#define gHciLeWriteRfPathCompensation_c             0x004DU
#endif /* gBLE50_d */

#if defined(gBLE51_d) && (gBLE51_d == TRUE)

#if defined(gBLE51_ConnectionlessCTESupport_d) && (gBLE51_ConnectionlessCTESupport_d == TRUE)
#define gHciLeSetConnectionlessCteTransmitParams_c          0x0051U
#define gHciLeSetConnectionlessCteTransmitEnable_c          0x0052U
#define gHciLeSetConnectionlessIqSamplingEnable_c           0x0053U
#endif /* (gBLE51_ConnectionlessCTESupport_d == TRUE) */

#if defined(gBLE51_ConnectionCTESupport_d) && (gBLE51_ConnectionCTESupport_d == TRUE)
#define gHciLeSetConnectionCteReceiveParams_c               0x0054U
#define gHciLeSetConnectionCteTransmitParams_c              0x0055U
#define gHciLeConnectionCteReqEnable_c                      0x0056U
#define gHciLeConnectionCteRspEnable_c                      0x0057U
#endif /* (gBLE51_ConnectionCTESupport_d == TRUE) */

#if ((defined(gBLE51_ConnectionCTESupport_d) && (gBLE51_ConnectionCTESupport_d == TRUE)) || \
    (defined(gBLE51_ConnectionlessCTESupport_d) && (gBLE51_ConnectionlessCTESupport_d == TRUE)))
#define gHciLeReadAntennaInformation_c                      0x0058U
#endif /* ((gBLE51_ConnectionCTESupport_d == TRUE) || (gBLE51_ConnectionlessCTESupport_d == TRUE)) */

#if defined(gBLE51_PeriodicAdvSyncTransferSupport_d) && (gBLE51_PeriodicAdvSyncTransferSupport_d == TRUE)
#define gHciLeSetPeriodicAdvReceiveEnable_c                 0x0059U
#define gHciLePeriodicAdvSyncTransfer_c                     0x005AU
#define gHciLePeriodicAdvSetInfoTransfer_c                  0x005BU
#define gHciLeSetPeriodicAdvSyncTransferParameters_c        0x005CU
#define gHciLeSetDefaultPeriodicAdvSyncTransferParameters_c 0x005DU
#endif /* (gBLE51_PeriodicAdvSyncTransferSupport_d == TRUE) */
#define gHciLeGenerateDhKeyV2_c                             0x005EU
#define gHciLeModifySleepClockAccuracy_c                    0x005FU
#endif /* gBLE51_d */

/* LE Set Host Feature command */
#define gHciLeSetHostFeature_c                              0x0074U

#if defined(gBLE52_LePowerControlSupport_d) && (gBLE52_LePowerControlSupport_d == TRUE)
#define gHciLeEnhancedReadTransmitPowerLevel_c              0x0076U
#define gHciLeReadRemoteTransmitPowerLevel_c                0x0077U
#define gHciLeSetPathLossReportingParams_c                  0x0078U
#define gHciLeSetPathLossReportingEnable_c                  0x0079U
#define gHciLeSetTransmitPowerReportingEnable_c             0x007AU
#endif /* gBLE52_LePowerControlSupport_d */

#if (defined(gBLE54_d) && (gBLE54_d == TRUE))
#if (defined(gBLE54_AdvertisingCodingSelectionSupport_d) && (gBLE54_AdvertisingCodingSelectionSupport_d == TRUE))
#define gHciLeSetExtAdvParamsV2_c                           0x007FU
#endif /* gBLE54_AdvertisingCodingSelectionSupport_d */
#endif /* gBLE54_d */

/* Temporary values for Channel Sounding support, values to be finalized in the core spec */
#if (defined(gBLE_ChannelSounding_d) && (gBLE_ChannelSounding_d == TRUE))
#define gHciLeReadLocalSupportedHadmCapabilities_c               0x0089U
#define gHciLeReadRemoteSupportedHadmCapabilities_c              0x008AU
#define gHciLeWriteRemoteCachedCapabilities_c                    0x008BU
#define gHciLeSetHadmSecurityEnable_c                            0x008CU
#define gHciLeHadmSetDefaultSettings_c                           0x008DU
#define gHciLeReadRemoteFaeTable_c                               0x008EU
#define gHciLeWriteRemoteFaeTable_c                              0x008FU
#define gHciLeCreateHadmConfig_c                                 0x0090U
#define gHciLeRemoveHadmConfig_c                                 0x0091U
#define gHciLeSetChannelClassification_c                         0x0092U
#define gHciLeHadmSetProcedureParameters_c                       0x0093U
#define gHciLeSetHadmProcedureEnable_c                           0x0094U
#define gHciLeHadmTest_c                                         0x0095U
#define gHciLeHadmTestEnd_c                                      0x0096U
#endif /* gBLE_ChannelSounding_d */

#define gHciLeSetDecisionData_c                     0x0080U
#define gHciLeSetDecisionInstructions_c             0x0081U

     /*! HCI OGF : 0x3F : Vendor Specific Debug Commands */
#define gHciVendorSpecificDebugCommands_c                       0x03FU
#define gHciSetTxPowerCommand_c                                 0x012DU
#define gHciVendorEnhancedNotificationCommand_c                 0x01F0U
#define gHciVendorSetExpmSupportedFeaturesCommand_c             0x009EU
#define gHciVendorGetConnectionParameters_c                     0x00A3U
        /*! HCI Commands OCF for OGF : 0x3F : Vendor Specific Debug Commands */
        /* Will be inherited from "vendor_debug_commands.h" */
/* Vendor specific command HCI_CMD_BLE_ADV_INDEX_CHANGE */
#define gHciVendorBleAdvIndexChangeCommand_c                     0x009DU

#define gHciVendorSetEncSessionKeyCommand_c                      0x00B0U

/* Vendor specific command LE_Set_Scheduler_Priority */
#define gHciVendorLeSetSchedulerPriority_c                       0x00B2U


#if defined(gBLE_ChannelSounding_d) && (gBLE_ChannelSounding_d == TRUE)
#define gHciVendorHadmConfig_c                                   0x00AFU
#endif

/* Macros used to build full HCI OpCodes */
#define HciCmdOpcode(cmdOgf, cmdOcf)        (((uint16_t)(cmdOgf) << SHIFT10) | (cmdOcf))
#define HciLeCmdOpcode(leCmdOcf)            (HciCmdOpcode(gHciLeControllerCommands_c, leCmdOcf))
#define HciInfoCmdOpcode(infoCmdOcf)        (HciCmdOpcode(gHciInformationalParameters_c, infoCmdOcf))
#define HciControllerCmdOpcode(ctrlCmdOcf)  (HciCmdOpcode(gHciControllerBasebandCommands_c, ctrlCmdOcf))
#define HciStatusCmdOpcode(statusCmdOcf)    (HciCmdOpcode(gHciStatusParameters_c, statusCmdOcf))
#define HciLeCsCmdOpcode(leCmdOcf)          (HciCmdOpcode(gHciLeControllerCommands_c, leCmdOcf))

#define BIT(x) (1 << (x))
#define BIT64(x) (((uint64_t)1) << (x))

#define gHciAddrTypePrivacyOffset_c      (0x02U)
#define gHciFilterPolicyPrivacyOffset_c  (BIT1)

#if defined(gBLE50_d) && (gBLE50_d == TRUE)
#define gHciAdvReportTypeMask_c     (0x1FU)
#define gHciAdvReportTypeShift_c    (0x00U)
#define gHciAdvReportStatusMask_c   (0x60U)
#define gHciAdvReportStatusShift_c  (0x05U)
#endif

/*! Bluetooth HCI Device Address Types */
#define gHciBleAddrTypePublic_c         (0x00U) /*!< Public Device Address - fixed into the Controller by the manufacturer. */
#define gHciBleAddrTypeRandom_c         (0x01U) /*!< Random Device Address - set by the Host into the Controller for privacy reasons. */
#define gHciBleAddrTypePublicIdentity_c (0x02U) /*!< Public Identity Address - corresponds to Resolvable Private Address. */
#define gHciBleAddrTypeRandomIdentity_c (0x03U) /*!< Random (static) Identity Address - corresponds to Resolvable Private Address. */
#if defined(gBLE50_d) && (gBLE50_d == TRUE)
#define gHciBleAddrTypeNone_c           (0xFFU) /*!< No address provided (anonymous advertisement) */
#endif

#define gAdvertisingCodingSelectionSupportShift_c           (5U * 8U)
#define gAdvertisingCodingSelectionHostSupportShift_c       (5U * 8U + 1U)
#define gLeDecisionBasedAdvertisingFilteringSupportShift_c  (5U * 8U + 2U)

#define gExtAdvDecisionDataTypeFlags_ResolvableTag_c  BIT0
/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
typedef enum
{
    gHciLeScanDisable_c   = 0x00,
    gHciLeScanEnable_c    = 0x01,
} hciLeScanEnable_t;

typedef enum
{
    gHciLeDuplicateFilteringDisable_c    = 0x00,
    gHciLeDuplicateFilteringEnable_c     = 0x01,
#if defined(gBLE50_d) && (gBLE50_d == TRUE)
    gHciLeDuplicateFilteringPeriodicEnable_c = 0x02,
#endif
} hciLeFilterDuplicates_t;

typedef enum
{
    gLinkLayerEncryptionOff_c    = 0x00,
    gLinkLayerEncryptionOn_c     = 0x01,
} hciLlEncryptionEnabled_t;

typedef union
{
    struct
    {
        uint16_t    ocf:        10;
        uint16_t    ogf:        6;
    };
    uint16_t    opCode;
} hciCommandOpCode_t;

typedef uint8_t hciEventCode_t;
typedef enum
{
    gHciDisconnectionCompleteEvent_c                    = 0x05,
    gHciEncryptionChangeEvent_c                         = 0x08,
    gHciReadRemoteVersionInformationCompleteEvent_c     = 0x0C,
    gHciCommandCompleteEvent_c                          = 0x0E,
    gHciCommandStatusEvent_c                            = 0x0F,
    gHciHardwareErrorEvent_c                            = 0x10,
    gHciNumberOfCompletedPacketsEvent_c                 = 0x13,
    gHciDataBufferOverflowEvent_c                       = 0x1A,
    gHciEncryptionKeyRefreshCompleteEvent_c             = 0x30,
    gHciLeMetaEvent_c                                   = 0x3E,
    gHciAuthenticatedPayloadTimeoutExpiredEvent_c       = 0x57,
    /* Vendor - Handover */
    gHciVendorHandoverMetaEvent_c                       = 0xFF,
} hciEventCode_tag;

typedef uint8_t hciLeMetaSubEventCode_t;
typedef enum
{
    gHciLeConnectionCompleteEvent_c                 = 0x01,
    gHciLeAdvertisingReportEvent_c                  = 0x02,
    gHciLeConnectionUpdateCompleteEvent_c           = 0x03,
    gHciLeReadRemoteFeaturesCompleteEvent_c         = 0x04,
    gHciLeLongTermKeyRequestEvent_c                 = 0x05,
    /* BLE v4.1 */
    gHciLeRemoteConnectionParameterRequestEvent_c   = 0x06,
    /* BLE v4.2 */
    gHciLeDataLengthChangeEvent_c                   = 0x07,
    gHciLeReadLocalP256PublicKeyCompleteEvent_c     = 0x08,
    gHciLeGenerateDhKeyCompleteEvent_c              = 0x09,
    gHciLeEnhancedConnectionCompleteEvent_c         = 0x0A,
    gHciLeDirectedAdvertisingReportEvent_c          = 0x0B,
    /* BLE v5.0 */
    gHciLePhyUpdateCompleteEvent_c                  = 0x0C,
    gHciLeExtAdvReportEvent_c                       = 0x0D,
    gHciLePeriodicAdvSyncEstbEvent_c                = 0x0E,
    gHciLePeriodicAdvReportEvent_c                  = 0x0F,
    gHciLePeriodicAdvSyncLostEvent_c                = 0x10,
    gHciLeScanTimeoutEvent_c                        = 0x11,
    gHciLeAdvSetTerminatedEvent_c                   = 0x12,
    gHciLeScanReqReceivedEvent_c                    = 0x13,
    gHciLeChannelSelAlgorithmEvent_c                = 0x14,
    /* BLE v5.1 */
    gHciLeConnectionlessIqReportEvent_c             = 0x15,
    gHciLeConnectionIqReportEvent_c                 = 0x16,
    gHciLeCteRequestFailedEvent_c                   = 0x17,
    gHciLePeriodicAdvSyncTransferReceived_c         = 0x18,

    /* BLE v5.2 */
    gHciLePathLossThresholdEvent_c                  = 0x20,
    gHciLeTransmitPowerReportingEvent_c             = 0x21,

    gHciLeSkdReportEvent_c                          = 0x40,

    /* BLE v6.0 - Channel Sounding */
    gHciLeHadmReadRemoteSupportedCapabilitiesCompleteEvent_c = 0x2C,
    gHciLeHadmReadRemoteFaeTableCompleteEvent_c              = 0x2D,
    gHciLeHadmSetSecurityEnableCompleteEvent_c               = 0x2E,
    gHciLeHadmConfigCompleteEvent_c                          = 0x2F,
    gHciLeHadmProcedureEnableCompleteEvent_c                 = 0x30,
    gHciLeHadmSubeventResultEvent_c                          = 0x31,
    gHciLeHadmSubeventResultContinueEvent_c                  = 0x32,
    gHciLeHadmTestEndCompleteEvent_c                         = 0x33,
    gHciLeHadmSubeventDebugResultEvent_c                     = 0xF0,

    gHciLeVendorEnhancedNotificationEvent_c                  = 0xFF,
} hciLeMetaSubEventCode_tag;

typedef uint64_t hciLeMetaSubEventMask_t;
typedef enum
{
    gHciLeConnectionCompleteEventMask_c                 = BIT((0 * 8) + 0),
    gHciLeAdvertisingReportEventMask_c                  = BIT((0 * 8) + 1),
    gHciLeConnectionUpdateCompleteEventMask_c           = BIT((0 * 8) + 2),
    gHciLeReadRemoteFeaturesCompleteEventMask_c         = BIT((0 * 8) + 3),
    gHciLeLongTermKeyRequestEventMask_c                 = BIT((0 * 8) + 4),
    /* BLE v4.1 */
    gHciLeRemoteConnectionParameterRequestEventMask_c   = BIT((0 * 8) + 5),
    /* BLE v4.2 */
    gHciLeDataLengthChangeEventMask_c                   = BIT((0 * 8) + 6),
    gHciLeReadLocalP256PublicKeyCompleteEventMask_c     = BIT((0 * 8) + 7),
    gHciLeGenerateDhKeyCompleteEventMask_c              = BIT((1 * 8) + 0),
    gHciLeEnhancedConnectionCompleteEventMask_c         = BIT((1 * 8) + 1),
    gHciLeDirectedAdvertisingReportEventMask_c          = BIT((1 * 8) + 2),
    /* BLE v5.0 */
    gHciLePhyUpdateCompleteEventMask_c                  = BIT((1 * 8) + 3),
    gHciLeExtAdvReportEventMask_c                       = BIT((1 * 8) + 4),
    gHciLePeriodicAdvSyncEstbEventMask_c                = BIT((1 * 8) + 5),
    gHciLePeriodicAdvReportEventMask_c                  = BIT((1 * 8) + 6),
    gHciLePeriodicAdvSyncLostEventMask_c                = BIT((1 * 8) + 7),
    gHciLeScanTimeoutEventMask_c                        = BIT((2 * 8) + 0),
    gHciLeAdvSetTerminatedEventMask_c                   = BIT((2 * 8) + 1),
    gHciLeScanReqReceivedEventMask_c                    = BIT((2 * 8) + 2),
    gHciLeChannelSelAlgorithmEventMask_c                = BIT((2 * 8) + 3),
    /* BLE v5.2 */
    gHciLePathLossThresholdEventMask_c                  = BIT((3 * 8) + 7),
    gHciLeTransmitPowerReportingEventMask_c             = BIT64((4 * 8) + 0),
} hciLeMetaSubEventMask_tag;

/* Event mask values assigned by CS IOP test plan r18 */
#define gHciLeCsReadRemoteSupportedCapabilitiesEventMask_c  0x2B
#define gHciLeCsReadRemoteFAETableEventMask_c               0x2C
#define gHciLeCsSecurityEnableEventMask_c                   0x2D
#define gHciLeCsConfigCompleteEventMask_c                   0x2E
#define gHciLeCsProcEnableCompleteEventMask_c               0x2F
#define gHciLeCsSubeventResultEventMask_c                   0x30
#define gHciLeCsSubeventResultContinueEventMask_c           0x31
#define gHciLeTestEndEventMask_c                            0x32

/* LL Feature bits */
/* Channel Sounding (Host Support) */
#define gChannelSoundingHostSupport_c                       0x2F /* 47 decimal */

typedef leSupportedFeatures_t hciLeSupportedFeatures_t;
typedef enum
{
    gHciLeEncryption_c                            = BIT((0 * 8) + 0), /* 0 */
    gHciConnectionParametersRequestProcedure_c    = BIT((0 * 8) + 1), /* 1 */
    gHciExtendedRejectIndication_c                = BIT((0 * 8) + 2), /* 2 */
    gHciPeripheralInitiatedFeaturesExchange_c     = BIT((0 * 8) + 3), /* 3 */
    gHciLePing_c                                  = BIT((0 * 8) + 4), /* 4 */
    gHciLeDataPacketLengthExtension_c             = BIT((0 * 8) + 5), /* 5 */
    gHciLlPrivacy_c                               = BIT((0 * 8) + 6), /* 6 */
    gHciExtendedScannerFilterPolicies_c           = BIT((0 * 8) + 7), /* 7 */
} hciLeSupportedFeatures_tag;

typedef uint8_t hciLeAdvertisingEnable_t;
typedef enum
{
    gHciLeAdvertisingDisable_c   = 0x00,
    gHciLeAdvertisingEnable_c    = 0x01,
} hciLeAdvertisingEnable_tag;

typedef uint8_t hciControllerToHostFlowControlEnable_t;
typedef enum
{
    gHciFlowControlOffAclPacketsOffHciSyncPackets_c  = 0x00,
    gHciFlowControlOnAclPacketsOffHciSyncPackets_c   = 0x01,
    gHciFlowControlOffAclPacketsOnHciSyncPackets_c   = 0x02,
    gHciFlowControlOnAclPacketsOnHciSyncPackets_c    = 0x03,
} hciControllerToHostFlowControlEnable_tag;

typedef uint8_t hciDataBufferOverflowLinkType_t;
typedef enum
{
    gHciDataBufferOverflowSynchronous_c  = 0x00,
    gHciDataBufferOverflowACL_c          = 0x01,
} hciDataBufferOverflowLinkType_tag;

/*! Bluetooth HCI Device Address Type - Size: 1 Octet
    Range: [gHciBleAddrTypePublic_c:gHciBleAddrTypeRandomIdentity_c, gHciBleAddrTypeNone_c] */
typedef uint8_t hciBleAddressType_t;

typedef enum {
    gHciScanAll_c                       =  0x00,    /*!< Scans all advertising packets. */
    gHciScanWithFilterAcceptList_c      =  0x01,    /*!< Scans advertising packets using the Filter Accept List. */
    gHciScanAllRpa_c                    =  0x02,
    gHciScanWithFilterAcceptListRpa_c   =  0x03
} hciBleScanningFilterPolicy_t;

#if defined(gBLE50_d) && (gBLE50_d == TRUE)
typedef uint16_t hciBleExtAdvReportEventType_t;
typedef enum
{
    gHciAdvReportConnectableAdv_c = 0x01,
    gHciAdvReportScannableAdv_c   = 0x02,
    gHciAdvReportDirectedAdv_c    = 0x04,
    gHciAdvReportScanResp_c       = 0x08,
    gHciAdvReportLegacyAdv_c      = 0x10,
} hciBleExtAdvReportEventType_tag;

typedef uint8_t hciBleExtAdvReportStatus_t;
typedef enum
{
    gHciAdvReportComplete_c   = 0x00,  /*!< Report complete */
    gHciAdvReportMoreToCome_c = 0x01,  /*!< Report incomplete, more data to come */
    gHciAdvReportTruncated_c  = 0x02,  /*!< Report incomplete, data truncated, no more to come */
} hciBleExtAdvReportStatus_tag;

typedef uint8_t hciBleExtDataOperation_t;
typedef enum
{
    gHciExtDataIntermediateFrag_c = 0x00, /*!< Intermediate fragment of extended advertising or scan response data. */
    gHciExtDataFirstFrag_c        = 0x01, /*!< First fragment of extended advertising or scan response data. */
    gHciExtDataLastFrag_c         = 0x02, /*!< Last fragment of extended advertising or scan response data. */
    gHciExtDataComplete_c         = 0x03, /*!< Complete extended advertising or scan response data. No fragmentation. */
    gHciExtDataUnchanged_c        = 0x04  /*!< Unchanged data (just update the Advertising DID of the periodic advertising. */
} hciBleExtDataOperation_tag;

typedef uint8_t hciBleFragmentPreference_t;
typedef enum
{
    gHciUseFragmentation_c = 0x00,
    gHciNoFragmentation_c  = 0x01
} hciBleFragmentPreference_tag;
#endif

typedef PACKED_STRUCT
{
    uint16_t    opCode;
    uint8_t     parameterTotalLength;
    uint8_t     aData[1];
}hciCommandPacket_t;

typedef PACKED_STRUCT
{
    uint16_t    handle      :12;
    uint16_t    pbFlag      :2;
    uint16_t    bcFlag      :2;
    uint16_t    dataTotalLength;
    uint8_t     aData[1];
}hciAclDataPacket_t;

typedef PACKED_STRUCT
{
    hciEventCode_t  eventCode;
    uint8_t     dataTotalLength;
    uint8_t     aData[0];
}hciEventPacket_t;

typedef bleResult_t hciErrorCode_t;

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#endif /* HCI_TYPES_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
