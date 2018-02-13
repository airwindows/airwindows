cmake_minimum_required(VERSION 3.9)
project(airwindows_ports)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-O2 -D__cdecl=)

include(Helpers.cmake)

add_subdirectory(include/vstsdk)
add_airwindows_plugin(Acceleration)
add_airwindows_plugin(BitShiftGain)
add_airwindows_plugin(GrooveWear)
add_airwindows_plugin(PurestConsoleBuss)
add_airwindows_plugin(PurestConsoleChannel)
add_airwindows_plugin(PurestDrive)
add_airwindows_plugin(PurestGain)
add_airwindows_plugin(TPDFDither)
