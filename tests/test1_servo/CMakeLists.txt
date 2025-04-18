cmake_minimum_required(VERSION 3.10)
project(servo_test)

set(CMAKE_CXX_STANDARD 17)


include_directories(${CMAKE_CURRENT_SOURCE_DIR})


add_executable(servo_test
    main.cpp
    PCA9685.cpp
)


target_link_libraries(servo_test pthread)
