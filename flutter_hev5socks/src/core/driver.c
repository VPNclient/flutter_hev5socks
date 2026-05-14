#include "vpnclient_driver.h"
#include "socks5.h"
#include "tun_device.h"

#ifdef _WIN32
#include <windows.h>
static HANDLE tun_fd = INVALID_HANDLE_VALUE;
#else
static int tun_fd = -1;
#endif

static struct socks5_conn socks;
static uint8_t packet_buf[65536];

bool vpnclient_driver_init(const struct vpnclient_config* config) {
    // 1. Создаем TUN/TAP устройство
    #ifdef _WIN32
    tun_fd = tap_create("vpn0", config->mtu);
    if (tun_fd == INVALID_HANDLE_VALUE) return false;
    #else
    tun_fd = tun_create("tun0", config->mtu);
    if (tun_fd < 0) return false;
    #endif

    // 2. Подключаемся к SOCKS5
    if (!socks5_connect(config->socks5_host, config->socks5_port,
                       config->username, config->password, &socks)) {
        return false;
    }

    return true;
}

void vpnclient_driver_start() {
    while (true) {
        // 1. Читаем пакет из TUN/TAP
        uint32_t len = sizeof(packet_buf);
        bool read_ok;
        
        #ifdef _WIN32
        read_ok = tap_read(tun_fd, packet_buf, &len);
        #else
        len = tun_read(tun_fd, packet_buf, len);
        read_ok = (len > 0);
        #endif

        if (!read_ok) continue;

        // 2. Отправляем через SOCKS5
        if (!socks5_send(socks.fd, packet_buf, len)) {
            break;
        }

        // 3. Принимаем ответ
        if (!socks5_recv(socks.fd, packet_buf, &len)) {
            break;
        }

        // 4. Пишем в TUN/TAP
        #ifdef _WIN32
        tap_write(tun_fd, packet_buf, len);
        #else
        tun_write(tun_fd, packet_buf, len);
        #endif
    }
}