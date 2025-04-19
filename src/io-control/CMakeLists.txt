cmake_minimum_required(VERSION 3.12)
project(switch_control)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找pkg-config
find_package(PkgConfig REQUIRED)

# 查找libgpiod库
pkg_check_modules(GPIOD REQUIRED IMPORTED_TARGET libgpiod)

# 添加可执行文件（注意处理文件名中的空格）
add_executable(switch_control "switch control 新版.cpp")

# 链接库
target_include_directories(switch_control PRIVATE ${GPIOD_INCLUDE_DIRS})
target_link_libraries(switch_control PRIVATE PkgConfig::GPIOD)

# 添加运行时权限提示（需手动执行）
install(TARGETS switch_control RUNTIME DESTINATION bin)
add_custom_command(TARGET switch_control POST_BUILD
    COMMENT "注意：运行程序可能需要sudo权限"
)