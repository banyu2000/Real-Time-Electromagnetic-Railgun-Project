cmake_minimum_required(VERSION 3.10)
project(ServoVisionQt2)

set(CMAKE_CXX_STANDARD 17)

# 查找 Qt5 包
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(OpenCV REQUIRED)
find_package(Qwt REQUIRED)

# 所有源文件
set(SOURCES
    main.cpp
    qt2.cpp
    libcam2opencv.cpp
    PCA9685.cpp
)

# 所有头文件（可选：对 IDE 支持友好）
set(HEADERS
    qt2.h
    libcam2opencv.h
    PCA9685.h
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 链接库
target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    ${OpenCV_LIBS}
)

# 包含头文件路径
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    target_link_libraries(${PROJECT_NAME} Qt5::Widgets ${OpenCV_LIBS} Qwt::Qwt)
)
