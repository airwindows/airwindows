function(add_airwindows_plugin name)
    file(GLOB plug_src
            src/${name}/*.h
            src/${name}/*.cpp)
    add_library(${name} MODULE ${plug_src} ${VSTSDK_SOURCES})
    target_include_directories(${name} PRIVATE ${VSTSDK_ROOT})
    set_target_properties(${name} PROPERTIES PREFIX "")
endfunction(add_airwindows_plugin)