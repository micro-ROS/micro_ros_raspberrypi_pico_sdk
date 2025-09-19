#include <rclc/types.h>
#include <hardware/pwm.h>

#include <rosidl_typesupport_c/visibility_control.h>

#include <std_msgs/msg/string.h>

#include <std_msgs/msg/int32.h>

#include <sensor_msgs/msg/detail/joint_state__type_support.h>
#include <sensor_msgs/msg/joint_state.h>

#include <rosidl_runtime_c/message_type_support_struct.h>
#include <rosidl_typesupport_interface/macros.h>
#include <rmw_microros/rmw_microros.h>
#include <rmw/rmw.h>

#include <rclc/publisher.h>

#include "ws2812_set_rgb.h"

#include "util.h"
#include "board.h"
#include "constant.h"
#include "enum.h"
#include "pico_uart_transports/pico_uart_transports.h"
#include "ros.h"
#include "joint.h"
#include "debug.h"

pico_uros_node_t node;
rcl_publisher_t log_publisher;
rcl_publisher_t trace_publisher;

//rcl_timer_t update_timer;
rcl_timer_t uptime_timer;
rcl_publisher_t uptime_publisher;

static status_t ping_agent(int timeout_ms, int nb_attempts) {
  rcl_ret_t ret = rmw_uros_ping_agent(timeout_ms, nb_attempts);
  if (ret != RCL_RET_OK)
    return KO;
  return OK;
}

/*static void update_timer_callback(rcl_timer_t *timer, int64_t last_call_time)
{
  empile_trace("update_timer_callback");

  for (int pin = 0; pin < NB_JOINTS; pin++) {
    if (joints[pin].goal_angle != joints[pin].actual_angle) {
      joints[pin].config.move_callback(&joints[pin]);
    }
  }

  depile_trace("update_timer_callback");
}*/

static void uptime_timer_callback(rcl_timer_t *timer, int64_t last_call_time)
{
  empile_trace("uptime_timer_callback");

  rcl_ret_t status;
  static std_msgs__msg__Int32 uptime = {};

  if (ping_agent(1000, 1) != OK) {
    ws2812_set_rgb(5, (uptime.data % 2) * 10, 0, 0);
  } else {
    ws2812_set_rgb(5, 0, (uptime.data % 2) * 10, 0);
    status = rcl_publish(&uptime_publisher, &uptime, NULL);
  }
  uptime.data = (uptime.data + 1);

  depile_trace("uptime_timer_callback");
}


void init_joint_subscriber_data() {
  joint_subscriber_data.position.size = NB_JOINTS;
  joint_subscriber_data.position.capacity = NB_JOINTS;
  joint_subscriber_data.position.data = joints_angle;
}

static void joint_subscriber_callback(const sensor_msgs__msg__JointState *inputs)
{
  empile_trace("servo_subscriber_callback");

  send_log("Calling joint subscriber");
  for (size_t i = 0; i < NB_BOARDS; i++) {
    send_log("Updating board %d", boards[i].joint_update == NULL);
    if (boards[i].joint_update != NULL) {
      boards[i].joint_update(&boards[i], inputs);
    }
  }
  /*for (size_t i = 0; i < NB_JOINTS; i++) {
    joints[i].angle = joints_angle[i];
    if (joints[i].config.move_callback != NULL) {
      joints[i].config.move_callback(&joints[i]);
    }
  }*/

  depile_trace("servo_subscriber_callback");
}



static status_t init_node(void) {
  node.allocator = rcl_get_default_allocator();
  if (ping_agent(TIMEOUT_MS, NB_ATTEMPTS) == KO) {
    return KO;
  }
  if (rclc_support_init(&node.support, 0, NULL, &node.allocator) != RCL_RET_OK) {
    return KO;
  }
  if (rclc_node_init_default(&node.node, NODE_NAME, "", &node.support) != RCL_RET_OK) {
    return KO;
  }
  if (rclc_executor_init(&node.executor, &node.support.context, NB_HANDLES, &node.allocator) != RCL_RET_OK) {
    return KO;
  }
  return OK;
}

status_t init_ros_pico(void) {
  rmw_uros_set_custom_transport(true, NULL, &pico_serial_transport_open, &pico_serial_transport_close, &pico_serial_transport_write, &pico_serial_transport_read);
  if (init_node() == KO)
    return KO;

  //rclc_timer_init_default(&update_timer, &node.support, UPDATE_TIMER_INTERVAL, &update_timer_callback);
  rclc_timer_init_default(&uptime_timer, &node.support, UPTIME_TIMER_INTERVAL, &uptime_timer_callback);
  rclc_executor_add_timer(&node.executor, &uptime_timer);
  rclc_publisher_init_default(&uptime_publisher, &node.node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32), UPTIME_PUBLISHER_TOPIC_NAME);

  sensor_msgs__msg__JointState__init(&joint_subscriber_data);
  init_joint_subscriber_data();
  rclc_subscription_init_default(&joint_subscriber, &node.node, ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, JointState), SERVO_SUBSCRIBER_TOPIC_NAME);
  rclc_executor_add_subscription(&node.executor, &joint_subscriber, &joint_subscriber_data, (void (*)(const void *))&joint_subscriber_callback, ON_NEW_DATA);

  rclc_publisher_init_default(&log_publisher, &node.node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String), LOG_PUBLISHER_TOPIC_NAME);
  rclc_publisher_init_default(&trace_publisher, &node.node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, String), TRACE_PUBLISHER_TOPIC_NAME);

  return OK;
}
