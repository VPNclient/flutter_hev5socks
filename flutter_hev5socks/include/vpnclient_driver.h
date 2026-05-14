#pragma once

#include <stdint.h>
#include <stdbool.h>

struct vpnclient_driver_config {
    const char* socks5_host;
    uint16_t socks5_port;
    const char* username;  // опционально
    const char* password;  // опционально
    uint16_t mtu;
};

bool vpnclient_driver_init(const struct vpn_config* config);
void vpnclient_driver_start();
void vpnclient_driver_stop();