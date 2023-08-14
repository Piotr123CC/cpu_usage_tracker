cmake_minimum_required(VERSION 3.10)

project(CpuUsageTracker)

set(${CMAKE_SOURCE_DIR}
    Inc
    Src)

set(SRC
    ${CMAKE_SOURCE_DIR}/Src/main.c
    ${CMAKE_SOURCE_DIR}/Src/threads.c
    ${CMAKE_SOURCE_DIR}/Src/algorithm.c
    ${CMAKE_SOURCE_DIR}/Src/others.c
    ${CMAKE_SOURCE_DIR}/Src/queue.c

    )

set(INCLUDES
    ${CMAKE_SOURCE_DIR}/Inc/main.h
    ${CMAKE_SOURCE_DIR}/Inc/threads.h
    ${CMAKE_SOURCE_DIR}/Inc/algorithm.h
    ${CMAKE_SOURCE_DIR}/Inc/others.h
    ${CMAKE_SOURCE_DIR}/Inc/queue.h
    )

add_executable(${PROJECT_NAME} ${SRC} ${INCLUDES})