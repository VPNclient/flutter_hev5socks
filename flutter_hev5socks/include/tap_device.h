// Windows
#pragma once

HANDLE tap_create(const char* name, uint16_t mtu);
bool tap_read(HANDLE h, uint8_t* buf, uint32_t* size);
bool tap_write(HANDLE h, const uint8_t* buf, uint32_t size);