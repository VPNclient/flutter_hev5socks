#include "hev_socks5_c_api.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define VERSION "1.0.0"
#define MAX_ERROR_LEN 256

// Внутренняя структура экземпляра
typedef struct {
    HevSocks5Config config;
    bool is_running;
    pthread_t thread;
    HevSocks5Stats stats;
    HevSocks5LogCallback log_callback;
    void* log_user_data;
    char last_error[MAX_ERROR_LEN];
    pthread_mutex_t mutex;
} HevSocks5InstanceImpl;

// Вспомогательные функции
static void log_message(HevSocks5InstanceImpl* impl, const char* level, const char* message) {
    if (impl && impl->log_callback) {
        impl->log_callback(level, message, impl->log_user_data);
    }
}

static void set_error(HevSocks5InstanceImpl* impl, const char* error) {
    if (impl) {
        strncpy(impl->last_error, error, MAX_ERROR_LEN - 1);
        impl->last_error[MAX_ERROR_LEN - 1] = '\0';
    }
}

// Функция потока туннеля
static void* tunnel_thread(void* arg) {
    HevSocks5InstanceImpl* impl = (HevSocks5InstanceImpl*)arg;
    
    log_message(impl, "INFO", "HevSocks5 tunnel thread started");
    
    // TODO: Здесь будет реальная реализация туннеля
    // Сейчас это заглушка для демонстрации
    while (impl->is_running) {
        pthread_mutex_lock(&impl->mutex);
        
        // Симуляция передачи данных
        impl->stats.tx_packets += 10;
        impl->stats.rx_packets += 8;
        impl->stats.tx_bytes += 1024;
        impl->stats.rx_bytes += 2048;
        
        pthread_mutex_unlock(&impl->mutex);
        
        // Спим 1 секунду
        struct timespec ts = {1, 0};
        nanosleep(&ts, NULL);
    }
    
    log_message(impl, "INFO", "HevSocks5 tunnel thread stopped");
    return NULL;
}

// Реализация API функций

HevSocks5Instance hev_socks5_create(void) {
    HevSocks5InstanceImpl* impl = (HevSocks5InstanceImpl*)calloc(1, sizeof(HevSocks5InstanceImpl));
    if (!impl) {
        return NULL;
    }
    
    impl->is_running = false;
    pthread_mutex_init(&impl->mutex, NULL);
    memset(&impl->stats, 0, sizeof(HevSocks5Stats));
    
    return (HevSocks5Instance)impl;
}

bool hev_socks5_init(HevSocks5Instance instance, const HevSocks5Config* config) {
    if (!instance || !config) {
        return false;
    }
    
    HevSocks5InstanceImpl* impl = (HevSocks5InstanceImpl*)instance;
    
    // Валидация конфигурации
    if (!config->socks5_server) {
        set_error(impl, "SOCKS5 server is required");
        return false;
    }
    
    // Копируем конфигурацию
    memcpy(&impl->config, config, sizeof(HevSocks5Config));
    
    // Дублируем строки
    if (config->socks5_server) {
        impl->config.socks5_server = strdup(config->socks5_server);
    }
    if (config->tun_name) {
        impl->config.tun_name = strdup(config->tun_name);
    }
    if (config->tun_address) {
        impl->config.tun_address = strdup(config->tun_address);
    }
    if (config->tun_gateway) {
        impl->config.tun_gateway = strdup(config->tun_gateway);
    }
    if (config->tun_netmask) {
        impl->config.tun_netmask = strdup(config->tun_netmask);
    }
    if (config->dns_server) {
        impl->config.dns_server = strdup(config->dns_server);
    }
    if (config->username) {
        impl->config.username = strdup(config->username);
    }
    if (config->password) {
        impl->config.password = strdup(config->password);
    }
    
    log_message(impl, "INFO", "HevSocks5 initialized successfully");
    return true;
}

bool hev_socks5_start(HevSocks5Instance instance) {
    if (!instance) {
        return false;
    }
    
    HevSocks5InstanceImpl* impl = (HevSocks5InstanceImpl*)instance;
    
    pthread_mutex_lock(&impl->mutex);
    
    if (impl->is_running) {
        pthread_mutex_unlock(&impl->mutex);
        set_error(impl, "Already running");
        return false;
    }
    
    impl->is_running = true;
    
    // Запускаем поток туннеля
    if (pthread_create(&impl->thread, NULL, tunnel_thread, impl) != 0) {
        impl->is_running = false;
        pthread_mutex_unlock(&impl->mutex);
        set_error(impl, "Failed to create tunnel thread");
        return false;
    }
    
    pthread_mutex_unlock(&impl->mutex);
    
    log_message(impl, "INFO", "HevSocks5 started");
    return true;
}

void hev_socks5_stop(HevSocks5Instance instance) {
    if (!instance) {
        return;
    }
    
    HevSocks5InstanceImpl* impl = (HevSocks5InstanceImpl*)instance;
    
    pthread_mutex_lock(&impl->mutex);
    
    if (!impl->is_running) {
        pthread_mutex_unlock(&impl->mutex);
        return;
    }
    
    impl->is_running = false;
    pthread_mutex_unlock(&impl->mutex);
    
    // Ждем завершения потока
    pthread_join(impl->thread, NULL);
    
    log_message(impl, "INFO", "HevSocks5 stopped");
}

bool hev_socks5_is_running(HevSocks5Instance instance) {
    if (!instance) {
        return false;
    }
    
    HevSocks5InstanceImpl* impl = (HevSocks5InstanceImpl*)instance;
    
    pthread_mutex_lock(&impl->mutex);
    bool running = impl->is_running;
    pthread_mutex_unlock(&impl->mutex);
    
    return running;
}

bool hev_socks5_get_stats(HevSocks5Instance instance, HevSocks5Stats* stats) {
    if (!instance || !stats) {
        return false;
    }
    
    HevSocks5InstanceImpl* impl = (HevSocks5InstanceImpl*)instance;
    
    pthread_mutex_lock(&impl->mutex);
    memcpy(stats, &impl->stats, sizeof(HevSocks5Stats));
    pthread_mutex_unlock(&impl->mutex);
    
    return true;
}

void hev_socks5_set_log_callback(HevSocks5Instance instance, 
                                  HevSocks5LogCallback callback, 
                                  void* user_data) {
    if (!instance) {
        return;
    }
    
    HevSocks5InstanceImpl* impl = (HevSocks5InstanceImpl*)instance;
    
    pthread_mutex_lock(&impl->mutex);
    impl->log_callback = callback;
    impl->log_user_data = user_data;
    pthread_mutex_unlock(&impl->mutex);
}

const char* hev_socks5_get_version(void) {
    return VERSION;
}

void hev_socks5_destroy(HevSocks5Instance instance) {
    if (!instance) {
        return;
    }
    
    HevSocks5InstanceImpl* impl = (HevSocks5InstanceImpl*)instance;
    
    // Останавливаем если запущено
    hev_socks5_stop(instance);
    
    // Освобождаем строки
    if (impl->config.socks5_server) free((void*)impl->config.socks5_server);
    if (impl->config.tun_name) free((void*)impl->config.tun_name);
    if (impl->config.tun_address) free((void*)impl->config.tun_address);
    if (impl->config.tun_gateway) free((void*)impl->config.tun_gateway);
    if (impl->config.tun_netmask) free((void*)impl->config.tun_netmask);
    if (impl->config.dns_server) free((void*)impl->config.dns_server);
    if (impl->config.username) free((void*)impl->config.username);
    if (impl->config.password) free((void*)impl->config.password);
    
    pthread_mutex_destroy(&impl->mutex);
    
    free(impl);
}

const char* hev_socks5_get_last_error(HevSocks5Instance instance) {
    if (!instance) {
        return "Invalid instance";
    }
    
    HevSocks5InstanceImpl* impl = (HevSocks5InstanceImpl*)instance;
    return impl->last_error;
}

