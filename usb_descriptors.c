#include "tusb.h"
#include "usb.h"
#include <stdint.h>

uint8_t const *tud_descriptor_device_cb(void) {
  return (uint8_t const *) &desc_device;
}

uint8_t const desc_configuration[] =
{
  // Config number, interface count, string index, total length, attribute, power in mA
  TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0x00, 100),

  // Interface number, string index, EP Out & EP In address, EP size
  TUD_AUDIO_MIC_FOUR_CH_DESCRIPTOR(/*_itfnum*/ ITF_NUM_AUDIO_CONTROL, /*_stridx*/ 0, /*_nBytesPerSample*/ CFG_TUD_AUDIO_FUNC_1_N_BYTES_PER_SAMPLE_TX, /*_nBitsUsedPerSample*/ CFG_TUD_AUDIO_FUNC_1_N_BYTES_PER_SAMPLE_TX*8, /*_epin*/ 0x80 | EPNUM_AUDIO, /*_epsize*/ CFG_TUD_AUDIO_EP_SZ_IN)
};

uint8_t const *tud_descriptor_configuration_cb(uint8_t index) {
  (void) index; // for multiple configurations
  return desc_configuration;
}

uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid);