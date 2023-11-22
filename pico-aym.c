//#include "hardware/uart.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include "hardware/adc.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
// #include "hardware/dma.h"
#include "hardware/pwm.h"

#include "pico/time.h"
#include "pico/multicore.h"

#include "aym.h"
//#include "players/ym-player.h"
#include "players/vgm-player.h"

// UART defines
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID uart0
#define BAUD_RATE 115200

// Use pins 16 and 17 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for
// information on GPIO assignments
#define UART_TX_PIN 0
#define UART_RX_PIN 1

#define CHANA 26
#define CHANB 27
#define CHANC 28

#define ADC_VREF 1
#define ADC_RANGE (1 << 12)
#define ADC_CONVERT (ADC_VREF / (ADC_RANGE - 1))

void core1_main();

int main() {
  // set_sys_clock_khz(16000, true);
  //stdio_init_all();

  // Set up our UART
  uart_init(UART_ID, BAUD_RATE);
  // Set the TX and RX pins by using the function select on the GPIO
  // Set datasheet for more information on function select
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

  //multicore_launch_core1(core1_main);

  //struct ym_data *ym_data = ym_load();
  struct vgm_data *vgm_data = vgm_load();
  if (vgm_data == NULL) {
    return 1;
  }

  // PWM clock
  gpio_set_function(CLOCK, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(CLOCK);
  float freq = (float)clock_get_hz(clk_sys);
  pwm_set_clkdiv(slice_num, freq / (vgm_data->clock * 2));
  pwm_set_wrap(slice_num, 1);
  pwm_set_gpio_level(CLOCK, 1);
  pwm_set_enabled(slice_num, true);

  // init gpio
  for (int i = 2; i < 10; i++) {
    gpio_init(i);
    gpio_set_dir(i, GPIO_OUT);
    gpio_put(i, false);
  }

  gpio_init(BC1);
  gpio_set_dir(BC1, GPIO_OUT);
  gpio_init(BDIR);
  gpio_set_dir(BDIR, GPIO_OUT);
  gpio_init(RESET);
  gpio_set_dir(RESET, GPIO_OUT);

  set_mode_inactive();

  gpio_put(RESET, false);
  sleep_ms(1);
  gpio_put(RESET, true);

  uart_puts(UART_ID, " Starting the player.\r\n");

  //int interval_ms = -1000000 / ym_data->rate;
  int interval_ms = -1000000 / 44100;
  struct repeating_timer timer;
  if (!add_repeating_timer_us(interval_ms, vgm_loop_callback, vgm_data, &timer)) {
    uart_puts(UART_ID, " Failed to add timer\r\n");
    return 1;
  }

  //uint8_t capture_buf[1024];

  while (1) {
    tight_loop_contents();
  }

  //while (1) {
    /*adc_select_input(0);
    uint adc_a_raw = adc_read();
    adc_select_input(1);
    uint adc_b_raw = adc_read();
    adc_select_input(2);
    uint adc_c_raw = adc_read();

    printf("%.2u\n", adc_a_raw * ADC_CONVERT);

    sleep_ms(10);*/

    /*struct ym_data *ym_data = ym_load();

    char rate_out[12];
    sprintf(rate_out, " rate: %i\r\n", ym_data->rate);
    uart_puts(UART_ID, rate_out);
    free(ym_data);*/

    //uart_puts(UART_ID, " Starting over.\r\n");
    //loop();
  //}

  return 0;
}

/*void core1_main() {
  tusb_init();

  
  adc_gpio_init(CHANA);
  adc_init();
  adc_select_input(0);
  adc_fifo_setup(true, true, 1, false, true);
  //adc_gpio_init(CHANA);
  //adc_gpio_init(CHANB);
  //adc_gpio_init(CHANC);

  while (1) {
    tud_task();
  }
}*/