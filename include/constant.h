#pragma once

#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 4

#define NB_BOARDS 1

#define NB_JOINTS 15

#define NB_INTERNAL_SENSORS   6
#define NB_INTERNAL_JOINTS    18

#define NB_ADAFRUIT_JOINTS    16

#define UPDATE_TIMER_INTERVAL RCL_MS_TO_NS(100)
#define UPTIME_TIMER_INTERVAL RCL_MS_TO_NS(1000)

#define MIN_PULSE 544.f
#define MAX_PULSE 2400.f

#define DEBUG_LED_PIN 18
#define FREQ_HZ       800000.0f

#define PWM_FREQ_HZ 20000
#define PWM_CLKDIV  125.f

#define NB_ATTEMPTS 120
#define TIMEOUT_MS  1000

#define MAX_NAME_LENGTH 128 // TODO REMOVE ?

#define MAX_LOG_SIZE  256

#define UPTIME_PUBLISHER_TOPIC_NAME "uptime_publisher"
#define LOG_PUBLISHER_TOPIC_NAME  "log_publisher"
#define TRACE_PUBLISHER_TOPIC_NAME  "trace_publisher"

#define SERVO_SUBSCRIBER_TOPIC_NAME  "servo_subscriber"

// SERVO_SUBSCRIBER + TIMER
#define NB_HANDLES    2
#define NODE_NAME     "pico_node"
