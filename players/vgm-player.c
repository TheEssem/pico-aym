#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"

#include "../aym.h"
#include "vgm-player.h"
#include "../tunes/build/vgm-file.h"

#define UART_ID uart0

struct vgm_data *vgm_load() {
  if (strncmp(VGM_FILE, "Vgm ", 4)) {
    uart_puts(UART_ID, " Invalid id\r\n");
    return NULL;
  }

  uint32_t *ver = (uint32_t *)(VGM_FILE + 8);
  if (*ver < (uint32_t)337) { // 1.51
    uart_puts(UART_ID, " Invalid version\r\n");
    return NULL;
  }

  struct vgm_data *data = malloc(sizeof(struct vgm_data));
  data->cur_pos = 0;
  data->delay = 0;
  data->done = false;

  uint32_t *total_samples = (uint32_t *)(VGM_FILE + 24);
  data->total_samples = *total_samples;
  uint32_t *loop_start = (uint32_t *)(VGM_FILE + 28);
  data->loop_start = *loop_start;
  uint32_t *loop_samples = (uint32_t *)(VGM_FILE + 32);
  data->loop_samples = *loop_samples;
  uint32_t *rate = (uint32_t *)(VGM_FILE + 36);
  data->rate = *rate;
  uint32_t *clock = (uint32_t *)(VGM_FILE + 116);
  data->clock = *clock;
  uint8_t *flags = (uint8_t *)(VGM_FILE + 121);
  data->flags = *flags;

  data->data_offset = (uint32_t)*(VGM_FILE + 52) + 52;
  data->data = VGM_FILE + data->data_offset;

  return data;
}

bool vgm_loop_callback(struct vgm_data *data) {
  if (data->cur_pos > data->total_samples || data->done) {
    data->cur_pos = data->loop_start ? data->loop_start + 28 - data->data_offset : 0;
    data->done = false;
  }

  if (data->delay > 0) {
    data->delay--;
    return true;
  }
  
  char cmd = data->data[data->cur_pos];

  if (cmd >= 0x70 && cmd <= 0x7F) { // wait n+1 samples
    data->delay = (cmd & 0x0F) + 1;
    data->cur_pos++;
    return true;
  }

  switch (cmd) {
    case 0xA0: // write to ay
      write_register(data->data[data->cur_pos + 1], data->data[data->cur_pos + 2]);
      data->cur_pos += 3;
      break;
    case 0x66: // end
      data->done = true;
      break;
    case 0x61: // wait
      data->delay = data->data[data->cur_pos + 1] | (data->data[data->cur_pos + 2] << 8);
      data->cur_pos += 3;
      break;
    case 0x62: // wait 735 samples
      data->delay = 735;
      data->cur_pos++;
      break;
    case 0x63: // wait 882 samples
      data->delay = 882;
      data->cur_pos++;
      break;
    case 0x94: // stop dac stream (????) ay/ym doesn't have a sample dac, furnace can output this for some reason
      data->cur_pos += 2;
      break;
    default: {
      char out[48];
      sprintf(out, " Unknown command %02X at offset %08X\r\n", cmd, data->cur_pos);
      uart_puts(UART_ID, out);
      break;
    }
  }

  return true;
}