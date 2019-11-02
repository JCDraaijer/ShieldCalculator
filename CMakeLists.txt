cmake_minimum_required(VERSION 3.14)
project(ShieldCalculator C)

set(CMAKE_C_STANDARD 99)

add_executable(ShieldCalculator main.c shieldbooster.h shieldbooster.c list.h list.c shieldgenerator.h shieldgenerator.c test.h test.c)