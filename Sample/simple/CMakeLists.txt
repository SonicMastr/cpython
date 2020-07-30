cmake_minimum_required(VERSION 3.0)

set(CMAKE_TOOLCHAIN_FILE "$ENV{DOLCESDK}/share/dolce.toolchain.cmake" CACHE PATH "DolceSDK toolchain file")
set(DOLCESDK_CMAKE_FILE "$ENV{DOLCESDK}/share/dolce.cmake" CACHE PATH "DolceSDK CMake functions file")
include("${DOLCESDK_CMAKE_FILE}" REQUIRED)

project(cpython-simple-example LANGUAGES C)

add_compile_options(
  -O3 -std=gnu11
  -Wall -Wextra -Wdouble-promotion -Wshadow -Wundef
  -fsingle-precision-constant -fno-common
)

include_directories(
  "$ENV{DOLCESDK}/arm-dolce-eabi/include/python2.7"
)

# Build self

set(ELF "simple.elf")
set(SELF "simple.self")

add_executable("${ELF}"
  main.c
)

target_link_libraries("${ELF}"
  python2.7
  z
  m
)

dolce_create_self("${SELF}"
  "${ELF}"
)

# Build VPK

set(VPK "cpython-simple-example.vpk")
set(TITLE_NAME "CPython Simple Example")
set(TITLE_ID "PYTH00001")
set(TITLE_VER "01.00")

dolce_create_vpk("${VPK}" "${TITLE_ID}" "${SELF}"
  NAME "${TITLE_NAME}"
  VERSION "${TITLE_VER}"
  FILE "$ENV{DOLCESDK}/arm-dolce-eabi/lib/python27.zip" lib/python27.zip
)
