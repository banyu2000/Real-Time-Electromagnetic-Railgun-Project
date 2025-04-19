cmake_minimum_required(VERSION 3.10)
project(SmartServoVisionSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter")

# 设置构建类型
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# 自动处理 Qt 的 MOC/UIC/资源文件
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 查找依赖包
find_package(PkgConfig REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(OpenCV REQUIRED)

# libcamera 绑定（用于回调）
pkg_check_modules(LIBCAMERA REQUIRED IMPORTED_TARGET libcamera)

# 包含头文件路径
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    ${LIBCAMERA_INCLUDE_DIRS}
)

# 添加源码文件
set(SOURCES
    main.cpp
    window.cpp
    PCA9685.cpp
)

set(HEADERS
    window.h
    PCA9685.h
)

# 生成可执行文件
add_executable(robot_app ${SOURCES} ${HEADERS})

# 链接库
target_link_libraries(robot_app
    Qt5::Widgets
    ${OpenCV_LIBS}
    PkgConfig::LIBCAMERA
    pthread
)

# 可选：编译信息提示
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "libcamera: ${LIBCAMERA_VERSION}")
