#include "tun_device.h"
#include <windows.h>
#include <winioctl.h>

HANDLE tap_create(const char* name, uint16_t mtu) {
    char path[256];
    snprintf(path, sizeof(path), "\\\\.\\Global\\%s.tap", name);
    
    HANDLE h = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, 
                          0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, NULL);
    
    // Активация интерфейса
    DWORD len;
    DeviceIoControl(h, TAP_CONTROL_CODE(6, 0), 
                   &(DWORD){1}, sizeof(DWORD), NULL, 0, &len, NULL);
    
    return h;
}1
