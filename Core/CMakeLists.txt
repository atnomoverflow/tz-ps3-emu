cmake_minimum_required(VERSION 3.29.2)
project(Core VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)

set(
    SourceFiles    
    ./src/filesystem/vfsLocalFile.cpp 
    
    ./src/memory/memorySegment.cpp 
    ./src/memory/memory.cpp 


    ./src/loader/elfLoader.cpp
    ./src/loader/loader.cpp
)
include_directories(include)
add_library(${PROJECT_NAME} STATIC ${SourceFiles})
target_link_libraries(${PROJECT_NAME} )