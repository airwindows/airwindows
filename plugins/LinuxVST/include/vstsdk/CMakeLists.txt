set(VSTSDK_ROOT
        ${CMAKE_CURRENT_SOURCE_DIR}
        CACHE STRING "VST SDK dir")

set(VSTSDK_SOURCES
        ${VSTSDK_ROOT}/vstplugmain.cpp
        ${VSTSDK_ROOT}/audioeffect.cpp
        ${VSTSDK_ROOT}/audioeffectx.cpp
        CACHE STRING "VST SDK source files")
