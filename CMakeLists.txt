CMAKE_MINIMUM_REQUIRED(VERSION 3.10.2)

PROJECT(advent2024 CXX)

SET (SRC_DIR ./src)

SET (SRC_FILES
		${SRC_DIR}/main.cpp
		${SRC_DIR}/day01.cpp
		${SRC_DIR}/day02.cpp
		${SRC_DIR}/day03.cpp
		${SRC_DIR}/day04.cpp
		${SRC_DIR}/day05.cpp
		${SRC_DIR}/day06.cpp
		${SRC_DIR}/day07.cpp
		${SRC_DIR}/day10.cpp
		${SRC_DIR}/day11.cpp
		${SRC_DIR}/day12.cpp
		${SRC_DIR}/day13.cpp
)

ADD_EXECUTABLE(advent2024 ${SRC_FILES})
SET_PROPERTY(TARGET advent2024 PROPERTY CXX_STANDARD 20)

FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/inputs/
	 DESTINATION ${CMAKE_BINARY_DIR}/inputs)