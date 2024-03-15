cmake_minimum_required(VERSION 3.20)
project(audisen)

set(CMAKE_CXX_STANDARD 14)

add_executable(audisen audisen_usb.c amp.c ams.c frame.c ftd2xx.lib usb.c)
