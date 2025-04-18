cmake_minimum_required(VERSION 3.16)
project(ServoCameraControl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 查找 Qt、OpenCV 和 Qwt 库
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Qwt REQUIRED)

# 包含头文件路径
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    ${Qwt_INCLUDE_DIRS}
    /usr/include/linux     # libcamera头文件
    /usr/include           # I2C 和其它基础头
)

# 添加源文件
add_executable(ServoCameraControl
    main.cpp
    window.cpp
    PCA9685.cpp
)

# 链接库
target_link_libraries(ServoCameraControl
    Qt5::Widgets
    ${OpenCV_LIBS}
    ${Qwt_LIBRARIES}
    pthread
    rt
    I2C    # 如果系统上这个名字无法识别，请换成 i2c-dev 或留空
)
