// Linux/macOS
#pragma once

int tun_create(const char* name, uint16_t mtu);
int tun_read(int fd, uint8_t* buf, uint32_t size);
int tun_write(int fd, const uint8_t* buf, uint32_t size);