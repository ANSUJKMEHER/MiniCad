cmake_minimum_required(VERSION 3.10)
project(MiniCAD CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src libs)

add_executable(MiniCAD 
    src/main.cpp
    src/Shapes.cpp
    src/GeometryMath.cpp
    src/SVGExporter.cpp
)

add_executable(MiniCADServer 
    src/server.cpp
    src/Shapes.cpp
    src/GeometryMath.cpp
    src/SVGExporter.cpp
)

if(WIN32)
    target_link_libraries(MiniCADServer wsock32 ws2_32)
endif()
