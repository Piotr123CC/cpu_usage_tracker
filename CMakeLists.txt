cmake_minimum_required(VERSION 3.10)

project(CpuUsageTracker LANGUAGES C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -lpthread -std=c99 -Wextra -Wall -fno-builtin -lm ")

set(CMAKE_LINK_FLAGS ${CMAKE_C_FLAGS} )
set(CMAKE_C_STANDARD 99)
set(SRC
    ${CMAKE_SOURCE_DIR}/Src/main.c
    ${CMAKE_SOURCE_DIR}/Src/mythreads.c
    ${CMAKE_SOURCE_DIR}/Src/algorithm.c
    ${CMAKE_SOURCE_DIR}/Src/others.c
    ${CMAKE_SOURCE_DIR}/Src/queue.c

    )

add_library(MyLib SHARED ${SRC})
target_include_directories(MyLib PUBLIC Inc)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC MyLib)