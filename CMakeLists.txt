cmake_minimum_required(VERSION 3.6)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

project(ServoVisionControl
    DESCRIPTION "Qt + OpenCV + libcamera + PCA9685 control"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Winvalid-pch -Wnon-virtual-dtor -Wextra -Wno-unused-parameter")

# 自动处理 Qt 元对象宏
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 查找依赖
find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Threads REQUIRED)  # 用于 std::thread
# 可选：Qwt 支持
find_package(Qwt REQUIRED)

# libcamera
pkg_check_modules(LIBCAMERA REQUIRED IMPORTED_TARGET libcamera)

# 日志输出
message(STATUS "libcamera:")
message(STATUS "    include path: ${LIBCAMERA_INCLUDE_DIRS}")
message(STATUS "    libraries: ${LIBCAMERA_LINK_LIBRARIES}")
message(STATUS "OpenCV:")
message(STATUS "    version: ${OpenCV_VERSION}")

# 头文件包含路径
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${LIBCAMERA_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
)

# 编译目标
add_executable(ServoVisionControl
    main.cpp
    window.cpp
    PCA9685.cpp
)

# 链接依赖
target_link_libraries(ServoVisionControl
    Qt5::Widgets
    ${OpenCV_LIBS}
    PkgConfig::LIBCAMERA
    Threads::Threads
    qwt-qt5
    cam2opencv
)

