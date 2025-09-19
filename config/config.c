#include "joint.h"
#include "board.h"

void dump_config(void) {
  memset(boards, 0, sizeof(boards));

  // Setup internal board
  board_t *board = &boards[0];
  board->init = &internal_init;
  board->joint_update = &internal_joint_update;

  // Setup servo

  joint_t *joint = board->data.internal.joints;

  joint->config.virtual_pin = 0;
  joint->config.physical_pin = 1;
  joint->config.physical_min_angle = 0;
  joint->config.physical_max_angle = 180;
  joint->config.min_angle = 0;
  joint->config.max_angle = 180;
  joint->config.default_angle = 90;
  joint->config.init_callback = &default_joint_init;
  joint->config.move_callback = &default_joint_move;
  joint++;
}
