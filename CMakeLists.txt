cmake_minimum_required(VERSION 3.16)
project(ServoCameraControl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 查找需要的库
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Qwt REQUIRED)

# 包含头文件
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/vision-recognition
    ${CMAKE_CURRENT_SOURCE_DIR}/servo-control
    ${OpenCV_INCLUDE_DIRS}
    ${Qwt_INCLUDE_DIRS}
    /usr/include/linux
    /usr/include
)

# 添加源文件
add_executable(ServoCameraControl
    main.cpp
    vision-recognition/window.cpp
    servo-control/PCA9685.cpp
)

# 链接库
target_link_libraries(ServoCameraControl
    Qt5::Widgets
    ${OpenCV_LIBS}
    ${Qwt_LIBRARIES}
    pthread
    rt
)
