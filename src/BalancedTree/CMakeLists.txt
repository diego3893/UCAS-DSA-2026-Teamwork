# 1. 设置 CMake 最小版本要求
cmake_minimum_required(VERSION 3.10)

# 2. 设置项目名称
project(BalancedTree)

# 3. 设置 C++ 标准 (例如 C++11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 4. 收集所有的源文件 (.cpp)
# 这里手动列出所有源文件。如果文件很多，也可以用 aux_source_directory 或 file(GLOB ...)
set(SOURCES
    src/main.cpp
    src/common/common.cpp
    src/tree/AVL.cpp
    src/display/display.cpp
)

# 5. 生成可执行文件
add_executable(BalancedTree ${SOURCES})

# 6. 【最关键的一步】告诉编译器去哪里找头文件！
# 这样配置后，编译器遇到 #include "xxx.h" 时，会自动去 include 目录下寻找
target_include_directories(BalancedTree PRIVATE ${CMAKE_SOURCE_DIR}/include)