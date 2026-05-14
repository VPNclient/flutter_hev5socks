#include "socks5.h"
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

bool socks5_connect(const char* host, uint16_t port, 
                   const char* user, const char* pass,
                   struct socks5_conn* out) {
    // 1. Создаем TCP соединение с SOCKS5 сервером
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) return false;

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = inet_addr(host)
    };

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(fd);
        return false;
    }

    // 2. Отправляем приветствие (handshake)
    uint8_t handshake[256];
    int handshake_len;
    
    if (user && pass) {
        // С аутентификацией
        handshake[0] = 0x05; // SOCKS версия
        handshake[1] = 0x02; // 2 метода
        handshake[2] = 0x00; // NO AUTH
        handshake[3] = 0x02; // USER/PASS
        handshake_len = 4;
    } else {
        // Без аутентификации
        handshake[0] = 0x05; // SOCKS версия
        handshake[1] = 0x01; // 1 метод
        handshake[2] = 0x00; // NO AUTH
        handshake_len = 3;
    }

    if (send(fd, handshake, handshake_len, 0) != handshake_len) {
        close(fd);
        return false;
    }

    // 3. Получаем ответ сервера
    uint8_t handshake_reply[2];
    if (recv(fd, handshake_reply, 2, 0) != 2) {
        close(fd);
        return false;
    }

    // 4. Проверяем выбранный метод
    if (handshake_reply[0] != 0x05) {
        close(fd);
        return false; // Не SOCKS5
    }

    if (handshake_reply[1] == 0x02) {
        // Требуется аутентификация
        if (!user || !pass) {
            close(fd);
            return false;
        }

        // 5. Отправляем данные аутентификации
        uint8_t auth[512];
        int user_len = strlen(user);
        int pass_len = strlen(pass);
        
        auth[0] = 0x01; // Версия USER/PASS
        auth[1] = user_len;
        memcpy(auth + 2, user, user_len);
        auth[2 + user_len] = pass_len;
        memcpy(auth + 3 + user_len, pass, pass_len);
        
        int auth_len = 3 + user_len + pass_len;
        
        if (send(fd, auth, auth_len, 0) != auth_len) {
            close(fd);
            return false;
        }

        // 6. Получаем ответ на аутентификацию
        uint8_t auth_reply[2];
        if (recv(fd, auth_reply, 2, 0) != 2) {
            close(fd);
            return false;
        }

        if (auth_reply[0] != 0x01 || auth_reply[1] != 0x00) {
            close(fd);
            return false; // Аутентификация не удалась
        }
    } else if (handshake_reply[1] != 0x00) {
        close(fd);
        return false; // Неподдерживаемый метод
    }

    // 7. Отправляем запрос на подключение (CONNECT)
    struct socks5_request {
        uint8_t ver;
        uint8_t cmd;
        uint8_t rsv;
        uint8_t atyp;
        uint32_t addr;
        uint16_t port;
    } __attribute__((packed)) request;

    request.ver = 0x05; // SOCKS5
    request.cmd = 0x01; // CONNECT
    request.rsv = 0x00;
    request.atyp = 0x01; // IPv4
    request.addr = inet_addr(host);
    request.port = htons(port);

    if (send(fd, &request, sizeof(request), 0) != sizeof(request)) {
        close(fd);
        return false;
    }

    // 8. Получаем ответ на запрос
    uint8_t reply[10];
    if (recv(fd, reply, 10, 0) != 10) {
        close(fd);
        return false;
    }

    if (reply[0] != 0x05 || reply[1] != 0x00) {
        close(fd);
        return false; // Ошибка подключения
    }

    out->fd = fd;
    return true;
}

bool socks5_send(int fd, const uint8_t* data, uint32_t len) {
    // Добавляем заголовок SOCKS5 для данных
    uint8_t packet[len + 3];
    packet[0] = 0x00; // RSV
    packet[1] = 0x00; // RSV
    packet[2] = 0x00; // FRAG
    
    memcpy(packet + 3, data, len);
    
    return send(fd, packet, len + 3, 0) == (len + 3);
}

bool socks5_recv(int fd, uint8_t* buf, uint32_t* len) {
    uint8_t header[3];
    if (recv(fd, header, 3, 0) != 3) {
        return false;
    }
    
    // Читаем только когда FRAG = 0
    if (header[2] != 0x00) {
        return false;
    }
    
    ssize_t received = recv(fd, buf, *len, 0);
    if (received <= 0) {
        return false;
    }
    
    *len = received;
    return true;
}