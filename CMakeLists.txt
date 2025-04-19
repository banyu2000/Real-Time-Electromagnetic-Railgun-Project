cmake_minimum_required(VERSION 3.10)
project(ServoVisionControl)

set(CMAKE_CXX_STANDARD 17)

# 查找 Qt5 组件
find_package(Qt5 REQUIRED COMPONENTS Widgets)

# 查找 OpenCV
find_package(OpenCV REQUIRED)

# 包含头文件目录
include_directories(
    ${Qt5Widgets_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 源文件列表
set(SOURCES
    main.cpp
    window.cpp
    PCA9685.cpp
)

# 头文件（供 IDE 可视化）
set(HEADERS
    window.h
    PCA9685.h
)

# 添加可执行文件
add_executable(ServoVisionControl ${SOURCES} ${HEADERS})

# 链接库
target_link_libraries(ServoVisionControl
    Qt5::Widgets
    ${OpenCV_LIBS}
)
