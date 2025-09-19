#include "hardware.h"
#include <pico/time.h>

uint32_t uptime_ms;

void uptime(void) {
  uptime_ms = to_ms_since_boot(get_absolute_time());
}
