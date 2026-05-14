#pragma once

#include <stdint.h>

struct socks5_conn {
    int fd;
    uint8_t auth_method;
};

bool socks5_connect(const char* host, uint16_t port, 
                   const char* user, const char* pass,
                   struct socks5_conn* out);

bool socks5_send(int fd, const uint8_t* data, uint32_t len);
bool socks5_recv(int fd, uint8_t* buf, uint32_t* len);