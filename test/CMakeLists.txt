cmake_minimum_required (VERSION 3.0)
project (doc_test)

# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)

set(src_file ${PROJECT_SOURCE_DIR}/test.cpp)

add_executable(${PROJECT_NAME} ${src_file}) 

# 添加库包含目录
include_directories(${PROJECT_SOURCE_DIR}/../include)

# 添加库
link_directories(${CMAKE_BINARY_DIR}/Debug)
target_link_libraries (doc_test libMetadata)

#设置输出目录
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}) 

#添加选项
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_C_FLAGS -Wall)  
    set(CMAKE_C_FLAGS -Wextra) 
    set(CMAKE_C_FLAGS -Werror) 
    set(CMAKE_C_FLAGS -fvisibility=hidden) 
    #set(CMAKE_C_FLAGS -pthread) 
    #set(CMAKE_CXX_FLAGS -pthread)  
    #set(CMAKE_EXE_LINKER_FLAGS -Wl,-rpath=.)
endif(CMAKE_COMPILER_IS_GNUCXX)

