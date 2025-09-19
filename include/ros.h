#pragma once

#include <hardware/pwm.h>

#include <rclc/node.h>
#include <rclc/executor.h>
#include <rcl/allocator.h>

#include "enum.h"

typedef struct pico_uros_node_s {
  rcl_node_t node;
  rcl_allocator_t allocator;
  rclc_support_t support;
  rclc_executor_t executor;
} pico_uros_node_t;

extern pico_uros_node_t node;
extern rcl_publisher_t log_publisher;
extern rcl_publisher_t trace_publisher;

extern rcl_timer_t uptime_timer;
extern rcl_timer_t update_timer;
extern rcl_publisher_t uptime_publisher;

extern pwm_config config;
status_t init_ros_pico(void);
void init_pwm(void);
void init_pwm_port(int16_t pin);
static status_t init_led(void);
static status_t init_i2c(int16_t sda_pin, int16_t scl_pin);
