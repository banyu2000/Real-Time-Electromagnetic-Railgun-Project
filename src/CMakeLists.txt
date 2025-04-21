cmake_minimum_required(VERSION 3.10)
project(ServoCam)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(PkgConfig REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

pkg_check_modules(LIBCAMERA REQUIRED IMPORTED_TARGET libcamera)
pkg_check_modules(GPIOD REQUIRED IMPORTED_TARGET libgpiodcxx)

include_directories(${CMAKE_SOURCE_DIR} ${LIBCAMERA_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

add_executable(ServoCam
    main.cpp
    window.cpp
    PCA9685.cpp
    libcam2opencv.cpp
)

target_link_libraries(ServoCam
    Qt5::Widgets
    ${OpenCV_LIBS}
    Threads::Threads
    PkgConfig::LIBCAMERA
    PkgConfig::GPIOD
    qwt-qt5
)