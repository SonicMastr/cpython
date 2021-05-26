cmake_minimum_required(VERSION 3.0)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "VITASDK toolchain file")
set(VITASDK_CMAKE_FILE "$ENV{VITASDK}/share/vita.cmake" CACHE PATH "VITASDK CMake functions file")
include("${VITASDK_CMAKE_FILE}" REQUIRED)

project(cpython-simple-example LANGUAGES C)

add_compile_options(
  -Ofast -std=gnu11
  -Wall -Wextra -Wdouble-promotion -Wshadow -Wundef
  -fsingle-precision-constant -fno-common
)

add_link_options(
  -Wl,--allow-multiple-definition
)

include_directories(
  "$ENV{VITASDK}/arm-vita-eabi/include/python2.7"
)

# Build self

set(ELF "simple.elf")
set(SELF "simple.self")

find_package(OpenSSL REQUIRED)

add_executable("${ELF}"
  main.c
)

target_link_libraries("${ELF}"
  python2.7
  pthread
  SceIofilemgr_stub
  SceNet_stub
  SceNetCtl_stub
  SceSsl_stub
  SceSysmodule_stub
  SceLibKernel_stub 
  ${OPENSSL_LIBRARIES}
  z
  m
)

vita_create_self(eboot.bin
  "${ELF}"
)

# Build VPK

set(VPK "cpython-simple-example.vpk")
set(TITLE_NAME "CPython Simple Example")
set(TITLE_ID "PYTH00001")
set(TITLE_VER "01.00")

vita_create_vpk("${VPK}" "${TITLE_ID}" eboot.bin
  NAME "${TITLE_NAME}"
  VERSION "${TITLE_VER}"
  FILE "$ENV{VITASDK}/arm-vita-eabi/lib/python27.zip" lib/python27.zip
  FILE ytdl.py ytdl.py
)
