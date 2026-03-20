cmake_minimum_required(VERSION 3.28)
project(CurlTesting LANGUAGES C)

add_executable(CurlTesting src/main.c)

target_link_libraries(CurlTesting PRIVATE curl)
