cmake_minimum_required(VERSION 3.28)
project(BasicCMake LANGUAGES C)
set(CMAKE_C_STANDARD 17)

set(SRC
    main.c
    printUtils.c
)

add_executable(BasicCMake ${SRC})
target_include_directories(BasicCMake PRIVATE "./")
