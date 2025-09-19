#pragma once

#include "joint.h"
#include "constant.h"

#include <stdint.h>

#include <sensor_msgs/msg/joint_state.h>

#include <hardware/pwm.h>
#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include <pico/stdlib.h>

struct i2c_pin_s {
  int16_t sda;
  int16_t scl;
};

struct adafruit_s {
  joint_t joints[NB_ADAFRUIT_JOINTS];
};

struct internal_s {
  pwm_config pwm_config;
  joint_t joints[NB_INTERNAL_JOINTS];
};



union board_pin_u {
  struct i2c_pin_s i2c;
};

union board_data_u {
  struct internal_s internal;
  struct adafruit_s adafruit;
};

typedef struct board_s {
  union board_pin_u pin;
  union board_data_u data;
  void (*init)(struct board_s *board);
  void (*joint_update)(struct board_s *board, const sensor_msgs__msg__JointState *inputs);
} board_t;

extern board_t boards[NB_BOARDS];

void internal_init(board_t *board);
void internal_joint_update(board_t *board, const sensor_msgs__msg__JointState *inputs);
void adafruit_init(board_t *board);
void adafruit_joint_update(board_t *board, const sensor_msgs__msg__JointState *inputs);

