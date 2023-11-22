#include <stdbool.h>
#include <stdint.h>
#include "pico/time.h"

struct ym_digi {
  uint32_t size;
  const char *data;
};

struct ym_data {
  uint32_t cur_frame;
  uint32_t frames;
  uint32_t attr;
  uint16_t digi_size;
  uint16_t rate;
  uint32_t clock;
  uint32_t loop;

  struct ym_digi *digis;
    
  const char *name;
  const char *author;
  const char *comment;
  const char *data;
};

struct ym_data *ym_load();
bool ym_loop_callback(struct repeating_timer *t);