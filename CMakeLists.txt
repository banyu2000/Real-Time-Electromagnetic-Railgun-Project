cmake_minimum_required(VERSION 3.10)
project(ServoVisionControl)

set(CMAKE_CXX_STANDARD 17)

# Qt5
find_package(Qt5 REQUIRED COMPONENTS Widgets)

# OpenCV
find_package(OpenCV REQUIRED)

# pthread 用于 std::thread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Qwt (可选，如果你真的用了 QwtThermo)
# find_package(Qwt REQUIRED)

# 头文件路径
include_directories(
    ${Qt5Widgets_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 源代码文件
set(SOURCES
    main.cpp
    window.cpp
    PCA9685.cpp
)

# 头文件
set(HEADERS
    window.h
    PCA9685.h
)

# 编译可执行文件
add_executable(ServoVisionControl ${SOURCES} ${HEADERS})

# 链接库
target_link_libraries(ServoVisionControl
    Qt5::Widgets
    ${OpenCV_LIBS}
    Threads::Threads
    # Qwt::Qwt  # 如果你启用了 Qwt，取消注释
)
