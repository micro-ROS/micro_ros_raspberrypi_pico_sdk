#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/i2c.h>

#include "ws2812.h"
#include "ws2812_set_rgb.h"

#include "board.h"
#include "enum.h"
#include "constant.h"
#include "joint.h"
#include "pwm.h"
#include "ros.h"

/*void init_pwm(void) {
  static bool init = false;

  if (init) {
    return;
  }
  init = true;
  config = pwm_get_default_config();

  pwm_config_set_wrap(&config, 20000);
  pwm_config_set_clkdiv(&config, 125.f);
}

void init_pwm_port(int16_t pin) {
  uint32_t sliceNum = pwm_gpio_to_slice_num(pin);

  gpio_set_function(pin, GPIO_FUNC_PWM);
  pwm_init(sliceNum, &config, true);
}

status_t init_i2c(int16_t sda_pin, int16_t scl_pin) {
  i2c_init(I2C, 1000000);
  gpio_set_function(sda_pin, GPIO_FUNC_I2C);
  gpio_set_function(scl_pin, GPIO_FUNC_I2C);
  gpio_pull_up(sda_pin);
  gpio_pull_up(scl_pin);
  return OK;
}*/

status_t init_led(void) {
  ws2812_init(pio1, DEBUG_LED_PIN, FREQ_HZ);
  ws2812_clear();
  ws2812_set_rgb(0, 10, 10, 0);
  return OK;
}

status_t init_stdio(void) {
  stdio_init_all();
  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }
  return OK;
}

status_t init_board(void) {
  for (int i = 0; i < NB_BOARDS; i++) {
    if (boards[i].init != NULL) {
      boards[i].init(&boards[i]);
    }
  }
  return OK;
}

status_t init_lucy(void) {
  init_led();
  init_stdio();
  init_board();
  return init_ros_pico();
}
