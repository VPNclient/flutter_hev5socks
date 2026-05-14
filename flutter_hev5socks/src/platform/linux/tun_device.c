#include "tun_device.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>

int tun_create(const char* name, uint16_t mtu) {
    int fd = open("/dev/net/tun", O_RDWR);
    struct ifreq ifr = {
        .ifr_flags = IFF_TUN | IFF_NO_PI
    };
    strncpy(ifr.ifr_name, name, IFNAMSIZ);

    ioctl(fd, TUNSETIFF, &ifr);
    
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "ip link set %s mtu %d", name, mtu);
    system(cmd);
    
    return fd;
}