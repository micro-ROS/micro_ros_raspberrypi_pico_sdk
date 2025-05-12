#ifndef MICRO_ROS_PICOSDK
#define MICRO_ROS_PICOSDK

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include <uxr/client/profile/transport/custom/custom_transport.h>

bool pico_wifi_transport_open(struct uxrCustomTransport *transport);
bool pico_wifi_transport_close(struct uxrCustomTransport *transport);
size_t pico_wifi_transport_write(struct uxrCustomTransport *transport, const uint8_t *buf, size_t len, uint8_t *err);
size_t pico_wifi_transport_read(struct uxrCustomTransport *transport, uint8_t *buf, size_t len, int timeout, uint8_t *err);

struct micro_ros_agent_locator
{
    ip_addr_t address;
    int port;
};

int scan_result(void *env, const cyw43_ev_scan_result_t *result) {
        if (result) {
            printf("ssid: %-32s rssi: %4d chan: %3d mac: %02x:%02x:%02x:%02x:%02x:%02x sec: %u\n",
                result->ssid, result->rssi, result->channel,
                result->bssid[0], result->bssid[1], result->bssid[2], result->bssid[3], result->bssid[4], result->bssid[5],
                result->auth_mode);
        }
        return 0;
    }

static inline bool set_microros_wifi_transports(char *ssid, char *pass, char *agent_ip, uint agent_port)
{
    stdio_init_all();
    stdio_usb_init();

    while(1)
    {
        if (cyw43_arch_init())
        {
            printf("failed to initialise\n");
            return 1;
        }
        sleep_ms(100);
        printf("Enabling Wifi\n");
        cyw43_arch_enable_sta_mode();
        printf("Higher power mode\n");
        cyw43_wifi_pm(&cyw43_state, CYW43_PERFORMANCE_PM);

        printf("Connecting to Wi-Fi...\n");
        printf("SSID: %s, Password: %s, Agent IP: %s, Agent Port: %u\n", ssid, pass, agent_ip, agent_port);

        int ret = cyw43_arch_wifi_connect_timeout_ms(ssid, pass, CYW43_AUTH_WPA2_AES_PSK, 12000);
        if (ret)
        {
            printf("failed to connect with error code: %d\n", ret);
            cyw43_arch_deinit();
        }
        else
        {
            printf("Connected.\n");
            break;
        }
    }
    printf("Connected.\n");

    static struct micro_ros_agent_locator locator;
    ipaddr_aton(agent_ip, &locator.address);
    locator.port = agent_port;

    rmw_uros_set_custom_transport(
        false,
        (void *)&locator,
        pico_wifi_transport_open,
        pico_wifi_transport_close,
        pico_wifi_transport_write,
        pico_wifi_transport_read);

    return 0;
}

#endif // MICRO_ROS_PICOSDK