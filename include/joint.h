#pragma once

#include "constant.h"

#include <stdint.h>

#include <rclc/rclc.h>
#include <rclc/types.h>
#include <rclc/subscription.h>
#include <sensor_msgs/msg/joint_state.h>

struct joint_s;
struct board_s;

typedef struct joint_config_s {
  int16_t virtual_pin;
  int16_t physical_pin;
  double physical_min_angle;
  double physical_max_angle;
  double min_angle;
  double max_angle;
  double default_angle;

  void (*init_callback)(struct board_s *board, struct joint_s *joint);
  void (*move_callback)(struct board_s *board, struct joint_s *joint);
} joint_config_t;

typedef struct joint_s {
  double angle;
  joint_config_t config;
} joint_t;

extern double joints_angle[NB_JOINTS];

extern rcl_subscription_t joint_subscriber;
extern sensor_msgs__msg__JointState joint_subscriber_data;

void default_joint_init(struct board_s *board, joint_t *joint);
void default_joint_move(struct board_s *board, joint_t *joint);
