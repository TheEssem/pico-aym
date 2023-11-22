#include "tusb.h"

#define SAMPLE_RATE 44100

void audio_task();

void usb_core_main() {
  tusb_init();

  while (1) {
    tud_task();

    audio_task();
  }
}