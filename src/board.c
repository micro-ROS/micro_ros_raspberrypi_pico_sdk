#include "board.h"

/* INTERNAL */

void internal_init(board_t *board) {
  board->data.internal.pwm_config = pwm_get_default_config();
  pwm_config_set_wrap(&board->data.internal.pwm_config, 20000);
  pwm_config_set_clkdiv(&board->data.internal.pwm_config, 125.f);

  for (int16_t i = 0; i < NB_INTERNAL_JOINTS; i++) {
    if (board->data.internal.joints[i].config.init_callback != NULL) {
      board->data.internal.joints[i].config.init_callback(board, &board->data.internal.joints[i]);
    }
  }
}

void internal_joint_update(board_t *board, const sensor_msgs__msg__JointState *inputs) {
  double new_angle;

  for (int i = 0; i < NB_INTERNAL_JOINTS; i++) {
    if (board->data.internal.joints[i].config.move_callback != NULL) {
      new_angle = inputs->position.data[board->data.internal.joints[i].config.virtual_pin];
      board->data.internal.joints[i].angle = new_angle;
      board->data.internal.joints[i].config.move_callback(board, &board->data.internal.joints[i]);
    }
  }
}

/* ADAFRUIT */

void adafruit_init(board_t *board) {
  i2c_init(i2c0, 1000000);
  gpio_set_function(board->pin.i2c.sda, GPIO_FUNC_I2C);
  gpio_set_function(board->pin.i2c.scl, GPIO_FUNC_I2C);
  gpio_pull_up(board->pin.i2c.sda);
  gpio_pull_up(board->pin.i2c.scl);

  for (int16_t i = 0; i < NB_ADAFRUIT_JOINTS; i++) {
    if (board->data.adafruit.joints[i].config.init_callback != NULL) {
      board->data.adafruit.joints[i].config.init_callback(board, &board->data.adafruit.joints[i]);
    }
  }
}

void adafruit_joint_update(board_t *board, const sensor_msgs__msg__JointState *inputs) {
  double new_angle;

  for (int i = 0; i < NB_ADAFRUIT_JOINTS; i++) {
    if (board->data.adafruit.joints[i].config.move_callback != NULL) {
      new_angle = inputs->position.data[board->data.internal.joints[i].config.virtual_pin];
      board->data.adafruit.joints[i].angle = new_angle;
      board->data.adafruit.joints[i].config.move_callback(board, &board->data.adafruitjoints[i]);
    }
  }
}



board_t boards[NB_BOARDS];
