cmake_minimum_required(VERSION 3.0)
project(kiwad VERSION 0.2.0)

include_directories(include)

add_library(kiwad src/kiwad.c)

add_executable(kwt tools/kwt.c)
target_link_libraries(kwt PUBLIC kiwad)