#include <stdbool.h>
#include <stdint.h>

struct vgm_data {
  uint32_t cur_pos;
  uint32_t delay;
  uint32_t cur_samples;
  uint32_t total_samples;
  uint32_t loop_start;
  uint32_t loop_samples;
  uint32_t rate;
  uint32_t clock;
  uint8_t flags;
  bool done;

  uint32_t data_offset;
  const char *data;
};

struct vgm_data *vgm_load();
void vgm_loop_callback(struct vgm_data *data);