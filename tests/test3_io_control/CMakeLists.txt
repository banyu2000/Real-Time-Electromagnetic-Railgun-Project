cmake_minimum_required(VERSION 3.15)
project(control-test CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找依赖库
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBGPIOD REQUIRED libgpiod)

# 定义可执行文件
add_executable(control-test
    control-test.cpp  # 你的主源文件
)

# 包含目录
target_include_directories(control-test PRIVATE
    ${LIBGPIOD_INCLUDE_DIRS}
)

# 链接库
target_link_libraries(control-test
    ${LIBGPIOD_LIBRARIES}
    pthread
)

# 安装规则（可选）
# install(TARGETS control-test DESTINATION bin)

# 错误处理
if(NOT LIBGPIOD_FOUND)
    message(FATAL_ERROR "libgpiod库未找到，请执行安装命令：\nsudo apt install libgpiod-dev")
endif()