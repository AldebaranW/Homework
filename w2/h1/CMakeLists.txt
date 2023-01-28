cmake_minimum_required(VERSION 3.24)
project(h1)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV 4 REQUIRED)
set(Eigen3_INCLUDE_DIRS /usr/include/Eigen)

add_executable(h1 main.cpp)
target_link_libraries(h1 ${OpenCV_LIBS})
