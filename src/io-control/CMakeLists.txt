cmake_minimum_required(VERSION 3.10)
project(switch_control)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找pkg-config工具
find_package(PkgConfig REQUIRED)

# 查找libgpiod库
pkg_check_modules(GPIOD REQUIRED IMPORTED_TARGET libgpiod)

# 创建可执行文件（处理带空格的文件名）
add_executable(switch_control 
    "switch control 新版.cpp"
)

# 链接库和包含目录
target_link_libraries(switch_control 
    PRIVATE 
    PkgConfig::GPIOD
)

# 添加警告选项
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(switch_control PRIVATE -Wall -Wextra)
endif()