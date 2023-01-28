cmake_minimum_required(VERSION 3.24)
project(h2)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV 4 REQUIRED)
set(Eigen3_INCLUDE_DIRS /usr/include/Eigen)

add_executable(h2 main.cpp)
target_link_libraries(h2 ${OpenCV_LIBS})

