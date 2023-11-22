#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"

#include "../aym.h"
#include "ym-player.h"
#include "../tunes/build/ym-file.h"

#define UART_ID uart0

uint16_t swap_uint16(uint16_t val) {
  return (val << 8) | (val >> 8 );
}

uint32_t swap_uint32(uint32_t val) {
  val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF); 
  return (val << 16) | (val >> 16);
}

struct ym_data *ym_load() {
  if (strncmp(YM_FILE, "YM5!", 4) && strncmp(YM_FILE, "YM6!", 4)) {
    uart_puts(UART_ID, " Invalid id\r\n");
    return NULL;
  }

  if (strncmp(YM_FILE + 4, "LeOnArD!", 8)) {
    uart_puts(UART_ID, " Invalid check\r\n");
    return NULL;
  }

  struct ym_data *data = malloc(sizeof(struct ym_data));
  data->cur_frame = 0;

  uint32_t *frames = (uint32_t *)(YM_FILE + 12);
  data->frames = swap_uint32(*frames);

  uint32_t *attr = (uint32_t *)(YM_FILE + 16);
  data->attr = swap_uint32(*attr);

  uint16_t *digiSize = (uint16_t *)(YM_FILE + 20);
  data->digi_size = swap_uint16(*digiSize);

  uint32_t *clock = (uint32_t *)(YM_FILE + 22);
  data->clock = swap_uint32(*clock);

  uint16_t *rate = (uint16_t *)(YM_FILE + 26);
  data->rate = swap_uint16(*rate);

  uint32_t *loop = (uint32_t *)(YM_FILE + 28);
  data->loop = swap_uint32(*loop);

  int alloc_size = 34;

  if (data->digi_size > 0) {
    struct ym_digi digis[data->digi_size];
    for (int i = 0; i < data->digi_size; i++) {
      uint32_t size;
      memcpy(&size, YM_FILE + alloc_size, 4);
      uint32_t size_swapped = swap_uint32(size);
      struct ym_digi *digi = malloc(sizeof(struct ym_digi));
      digi->size = size_swapped;
      //memcpy(digi->data, YM_FILE + alloc_size + 4, size_swapped);
      digi->data = YM_FILE + alloc_size + 4;
      digis[i] = *digi;
      alloc_size += size_swapped + 4;
    }
    data->digis = digis;
  } else {
    data->digis = NULL;
  }

  const char *name = YM_FILE + alloc_size;
  alloc_size += strlen(name) + 1;

  const char *author = YM_FILE + alloc_size;
  alloc_size += strlen(author) + 1;

  const char *comment = YM_FILE + alloc_size;
  alloc_size += strlen(comment) + 1;

  data->name = name;
  data->author = author;
  data->comment = comment;
  data->data = YM_FILE + alloc_size;

  return data;
}

bool ym_loop_callback(struct repeating_timer *t) {
  struct ym_data *data = t->user_data;

  char out[26];
  sprintf(out, " Running frame %i\r\n", data->cur_frame);
  uart_puts(UART_ID, out);
  
  if (data->cur_frame > data->frames) {
    data->cur_frame = data->loop;
  }

  if (data->attr == 1) { // interleaved
    for (int i = 0; i < 14; i++) {
      write_register(i, *(data->data + data->cur_frame + (i * data->frames)));
    }
  } else { // non-interleaved
    for (int i = 0; i < 14; i++) {
      write_register(i, *(data->data + (data->cur_frame * 16) + i));
    }
  }
  uart_puts(UART_ID, " frame done\r\n");
  data->cur_frame++;
  return true;
}