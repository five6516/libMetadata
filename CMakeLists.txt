cmake_minimum_required (VERSION 3.0)
project (libMetadata)

# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)

set(src_file ${PROJECT_SOURCE_DIR}/src/metadata.cpp  ${PROJECT_SOURCE_DIR}/src/metadata_tag_info.cpp)

add_library(${PROJECT_NAME} SHARED ${src_file}) 

# 添加包含目录
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/3rd)

# 设置输出目录
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Bin)
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Bin)

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


# 添加子目录
ADD_SUBDIRECTORY(test)

