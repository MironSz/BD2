cmake_minimum_required(VERSION 3.5)
project(bd)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS)
set(SOURCE_FILES main.cpp main.cpp)
add_executable(bd ${SOURCE_FILES})
target_link_libraries(bd pq pqxx)