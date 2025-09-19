#include "joint.h"
#include "pwm.h"
#include "util.h"
#include "board.h"

double joints_angle[NB_JOINTS] = {};

rcl_subscription_t joint_subscriber = {};
sensor_msgs__msg__JointState joint_subscriber_data = {};

rcl_subscription_t joint_config_subscriber = {};

void internal_joint_init(board_t *board, joint_t *joint) {
  int sliceNum = pwm_gpio_to_slice_num(joint->config.physical_pin);

  gpio_set_function(joint->config.physical_pin, GPIO_FUNC_PWM);
  pwm_init(sliceNum, &board->data.internal.pwm_config, true);
}

void internal_joint_move(board_t *board, joint_t *joint) {
  double us = map(joint->angle, joint->config.physical_min_angle, joint->config.physical_max_angle, MIN_PULSE, MAX_PULSE);
  pwm_set_gpio_level(joint->config.physical_pin, us);
}

void adafruit_joint_init(boad_t *board, joint_t *joint) {

}

void adafruit_joint_move(board_t *board, joint_t *joint) {

}
