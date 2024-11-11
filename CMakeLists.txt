cmake_minimum_required(VERSION 3.1)
Project(hello)
find_package(OpenCV REQUIRED)

add_executable(aVideoTest videoTest1.0.cpp)

target_link_libraries(aVideoTest ${OpenCV_LIBS})

include_directories(${OpenCV_INCLUDE_DIRS})

