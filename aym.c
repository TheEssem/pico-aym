#include "pico/stdlib.h"
#include "aym.h"

void set_mode_inactive() {
  gpio_put(BC1, false);
  gpio_put(BDIR, false);
}

void set_mode_latch() {
  gpio_put(BC1, true);
  gpio_put(BDIR, true);
}

void set_mode_write() {
  gpio_put(BC1, false);
  gpio_put(BDIR, true);
}

void set_mode_read() {
  gpio_put(BC1, true);
  gpio_put(BDIR, false);
}

void write_register(char reg, char value) {
  // set register
  set_mode_latch();
  gpio_put_masked(0xff << 2, reg << 2);
  busy_wait_us_32(1);
  set_mode_inactive();

  // write value
  set_mode_write();
  gpio_put_masked(0xff << 2, value << 2);
  busy_wait_us_32(1);
  set_mode_inactive();
}