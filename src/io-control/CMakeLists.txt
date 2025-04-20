cmake_minimum_required(VERSION 3.15)
project(gpio_control CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBGPIOD REQUIRED libgpiod)

add_executable(gpio_control
    main.cpp
)

target_include_directories(gpio_control PRIVATE
    ${LIBGPIOD_INCLUDE_DIRS}
)

target_link_libraries(gpio_control
    ${LIBGPIOD_LIBRARIES}
    pthread
)

install(TARGETS gpio_control DESTINATION bin)

if(NOT LIBGPIOD_FOUND)
    message(FATAL_ERROR "libgpiod required: sudo apt install libgpiod-dev")
endif()