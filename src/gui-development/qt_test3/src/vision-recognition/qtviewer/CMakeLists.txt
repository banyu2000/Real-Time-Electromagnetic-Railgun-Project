cmake_minimum_required(VERSION 3.10)
project(QtViewerWithGPIO LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Winvalid-pch -Wnon-virtual-dtor -Wextra -Wno-unused-parameter")

# 必需组件
find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# libcamera
pkg_check_modules(LIBCAMERA REQUIRED IMPORTED_TARGET libcamera)

# libgpiod
pkg_check_modules(GPIOD REQUIRED libgpiod)

# 自动 MOC / UIC / RCC 支持
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 输出信息
message(STATUS "OpenCV: ${OpenCV_VERSION}")
message(STATUS "libcamera: ${LIBCAMERA_VERSION}")
message(STATUS "libgpiod: ${GPIOD_VERSION}")

# 包含头文件目录
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${LIBCAMERA_INCLUDE_DIRS}
    ${OPENCV_INCLUDE_DIRS}
    ${GPIOD_INCLUDE_DIRS}
)

# 添加可执行文件
add_executable(qtviewer
    main.cpp
    window.cpp
    GpioControl.cpp
)

# 链接依赖库
target_link_libraries(qtviewer
    Qt5::Widgets
    ${OpenCV_LIBS}
    PkgConfig::LIBCAMERA
    ${GPIOD_LIBRARIES}
    qwt-qt5
    cam2opencv
)
