cmake_minimum_required(VERSION 3.15)
project(hev_socks5 VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Опции сборки
option(BUILD_SHARED_LIBS "Build shared library" ON)
option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_TESTS "Build tests" OFF)

# Исходные файлы
set(SOURCES
    src/core/hev_socks5_c_api.c
)

# Заголовочные файлы
set(HEADERS
    include/hev_socks5_c_api.h
)

# Создание библиотеки
add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Заголовочные директории
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Линковка с pthread
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

# Установка
install(TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${HEADERS}
    DESTINATION include
)

# Примеры
if(BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

# Тесты
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
