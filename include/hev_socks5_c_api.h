#ifndef HEV_SOCKS5_C_API_H
#define HEV_SOCKS5_C_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

// Типы данных
typedef void* HevSocks5Instance;

typedef struct {
    const char* socks5_server;      // SOCKS5 сервер (например, "127.0.0.1:1080")
    const char* tun_name;           // Имя TUN устройства
    const char* tun_address;        // IP адрес TUN (например, "10.0.0.2")
    const char* tun_gateway;        // Шлюз TUN (например, "10.0.0.1")
    const char* tun_netmask;        // Маска сети (например, "255.255.255.0")
    const char* dns_server;         // DNS сервер (например, "8.8.8.8")
    uint16_t tun_mtu;               // MTU (обычно 1500)
    int32_t tun_fd;                 // File descriptor TUN устройства (-1 для автосоздания)
    bool enable_ipv6;               // Включить IPv6
    const char* username;           // SOCKS5 username (опционально)
    const char* password;           // SOCKS5 password (опционально)
} HevSocks5Config;

typedef struct {
    uint64_t tx_packets;            // Отправлено пакетов
    uint64_t rx_packets;            // Получено пакетов
    uint64_t tx_bytes;              // Отправлено байт
    uint64_t rx_bytes;              // Получено байт
} HevSocks5Stats;

// Callback для логов
typedef void (*HevSocks5LogCallback)(const char* level, const char* message, void* user_data);

// API функции

/**
 * Создать экземпляр HevSocks5
 * @return Указатель на экземпляр или NULL при ошибке
 */
HevSocks5Instance hev_socks5_create(void);

/**
 * Инициализировать HevSocks5 с конфигурацией
 * @param instance Экземпляр
 * @param config Конфигурация
 * @return true при успехе, false при ошибке
 */
bool hev_socks5_init(HevSocks5Instance instance, const HevSocks5Config* config);

/**
 * Запустить туннель
 * @param instance Экземпляр
 * @return true при успехе, false при ошибке
 */
bool hev_socks5_start(HevSocks5Instance instance);

/**
 * Остановить туннель
 * @param instance Экземпляр
 */
void hev_socks5_stop(HevSocks5Instance instance);

/**
 * Проверить, запущен ли туннель
 * @param instance Экземпляр
 * @return true если запущен
 */
bool hev_socks5_is_running(HevSocks5Instance instance);

/**
 * Получить статистику
 * @param instance Экземпляр
 * @param stats Указатель на структуру для статистики
 * @return true при успехе
 */
bool hev_socks5_get_stats(HevSocks5Instance instance, HevSocks5Stats* stats);

/**
 * Установить callback для логов
 * @param instance Экземпляр
 * @param callback Функция callback
 * @param user_data Пользовательские данные
 */
void hev_socks5_set_log_callback(HevSocks5Instance instance, 
                                  HevSocks5LogCallback callback, 
                                  void* user_data);

/**
 * Получить версию библиотеки
 * @return Строка с версией
 */
const char* hev_socks5_get_version(void);

/**
 * Уничтожить экземпляр
 * @param instance Экземпляр
 */
void hev_socks5_destroy(HevSocks5Instance instance);

/**
 * Получить последнюю ошибку
 * @param instance Экземпляр
 * @return Строка с описанием ошибки или NULL
 */
const char* hev_socks5_get_last_error(HevSocks5Instance instance);

#ifdef __cplusplus
}
#endif

#endif // HEV_SOCKS5_C_API_H

