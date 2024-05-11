#pragma once

//#define BOARD_DEVICE_RHPORT_NUM     0
#define BOARD_DEVICE_RHPORT_SPEED   OPT_MODE_HIGH_SPEED
#define CFG_TUSB_RHPORT0_MODE      (OPT_MODE_DEVICE | BOARD_DEVICE_RHPORT_SPEED)
#define CFG_TUD_ENDPOINT0_SIZE    64

//------------- CLASS -------------//
#define CFG_TUD_AUDIO             1
#define CFG_TUD_CDC               0
#define CFG_TUD_MSC               0
#define CFG_TUD_HID               0
#define CFG_TUD_MIDI              0
#define CFG_TUD_VENDOR            0

//--------------------------------------------------------------------
// AUDIO CLASS DRIVER CONFIGURATION
//--------------------------------------------------------------------

// Have a look into audio_device.h for all configurations
#define CFG_TUD_AUDIO_FUNC_1_SAMPLE_RATE              44100

#define CFG_TUD_AUDIO_FUNC_1_DESC_LEN                 TUD_AUDIO_MIC_FOUR_CH_DESC_LEN

#define CFG_TUD_AUDIO_FUNC_1_N_AS_INT                 1
#define CFG_TUD_AUDIO_FUNC_1_CTRL_BUF_SZ              64

#define CFG_TUD_AUDIO_ENABLE_EP_IN                    1
#define CFG_TUD_AUDIO_FUNC_1_N_BYTES_PER_SAMPLE_TX    2         // This value is not required by the driver, it parses this information from the descriptor once the alternate interface is set by the host - we use it for the setup
#define CFG_TUD_AUDIO_FUNC_1_N_CHANNELS_TX            4         // This value is not required by the driver, it parses this information from the descriptor once the alternate interface is set by the host - we use it for the setup
#define CFG_TUD_AUDIO_EP_SZ_IN                        TUD_AUDIO_EP_SIZE(CFG_TUD_AUDIO_FUNC_1_SAMPLE_RATE, CFG_TUD_AUDIO_FUNC_1_N_BYTES_PER_SAMPLE_TX, CFG_TUD_AUDIO_FUNC_1_N_CHANNELS_TX)

#define CFG_TUD_AUDIO_ENABLE_ENCODING                 1
#define CFG_TUD_AUDIO_EP_IN_FLOW_CONTROL              1

#if CFG_TUD_AUDIO_ENABLE_ENCODING

#define CFG_TUD_AUDIO_FUNC_1_EP_IN_SZ_MAX             CFG_TUD_AUDIO_EP_SZ_IN
#define CFG_TUD_AUDIO_FUNC_1_EP_IN_SW_BUF_SZ          CFG_TUD_AUDIO_EP_SZ_IN

#define CFG_TUD_AUDIO_ENABLE_TYPE_I_ENCODING          1
#define CFG_TUD_AUDIO_FUNC_1_CHANNEL_PER_FIFO_TX      2         // One I2S stream contains two channels, each stream is saved within one support FIFO - this value is currently fixed, the driver does not support a changing value
#define CFG_TUD_AUDIO_FUNC_1_N_TX_SUPP_SW_FIFO        (CFG_TUD_AUDIO_FUNC_1_N_CHANNELS_TX / CFG_TUD_AUDIO_FUNC_1_CHANNEL_PER_FIFO_TX)
#define CFG_TUD_AUDIO_FUNC_1_TX_SUPP_SW_FIFO_SZ       4 * (CFG_TUD_AUDIO_EP_SZ_IN / CFG_TUD_AUDIO_FUNC_1_N_TX_SUPP_SW_FIFO) // Minimum 4*EP size is needed for flow control

#else

#define CFG_TUD_AUDIO_FUNC_1_EP_IN_SZ_MAX             CFG_TUD_AUDIO_EP_SZ_IN
#define CFG_TUD_AUDIO_FUNC_1_EP_IN_SW_BUF_SZ          4 * CFG_TUD_AUDIO_EP_SZ_IN    // Minimum 4*EP size is needed for flow control

#endif