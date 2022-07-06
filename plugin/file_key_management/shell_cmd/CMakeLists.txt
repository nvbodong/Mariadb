cmake_minimum_required(VERSION 3.17)
project(shell_cmd)

set(CMAKE_CXX_STANDARD 17)
include_directories(.)

#add_executable(shell_cmd fs.cc timer.cc string_helper.cc shell.cc test_fs.cc)
add_library(shell_cmd SHARED fs.cc timer.cc string_helper.cc port.cpp shell.cc )