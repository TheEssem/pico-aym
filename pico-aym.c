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

#define CLK_IRQ 20

#define ADC_VREF 1
#define ADC_RANGE (1 << 12)
#define ADC_CONVERT (ADC_VREF / (ADC_RANGE - 1))

void core1_main();

static struct vgm_data *vgm_data;
static float freq;

void on_pwm_wrap() {
  pwm_clear_irq(pwm_gpio_to_slice_num(CLK_IRQ));

  vgm_loop_callback(vgm_data);
}

int main() {
  set_sys_clock_pll(882000000, 5, 4);

  // Set up our UART
  uart_init(UART_ID, BAUD_RATE);
  // Set the TX and RX pins by using the function select on the GPIO
  // Set datasheet for more information on function select
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

  //multicore_launch_core1(core1_main);

  vgm_data = vgm_load();
  if (vgm_data == NULL) {
    return 1;
  }

  // PWM clock
  gpio_set_function(CLOCK, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(CLOCK);
  freq = (float)clock_get_hz(clk_sys);
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

  // VGM clock
  gpio_set_function(CLK_IRQ, GPIO_FUNC_PWM);
  uint vgm_slice = pwm_gpio_to_slice_num(CLK_IRQ);
  pwm_clear_irq(vgm_slice);
  pwm_set_irq_enabled(vgm_slice, true);
  irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
  irq_set_enabled(PWM_IRQ_WRAP, true);
  pwm_config config = pwm_get_default_config();
  pwm_config_set_clkdiv(&config, 2500);
  pwm_config_set_wrap(&config, 4);
  pwm_init(vgm_slice, &config, true);

  while (1) {
    tight_loop_contents();
  }

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