
#your project's CmakeLists.txt 
cmake_minimum_required(VERSION 3.13)

#1.set SDK path (environment variable or explicity path )
set(PICO_SDK_PATH "C:/Users/ishar/.pico-sdk/sdk/2.2.0")

#2.include the SDK's top-level Cmake file.
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)
set(PROJECT hello_uart)

project(${PROJECT} C CXX ASM)
# set(picotoolVersion 2.2.0-a4)
set(picotool_DIR "C:/Users/ishar/.pico-sdk/picotool/2.2.0-a4/picotool" CACHE PATH "Path to picotool")

#3.INitilize the sdk 
pico_sdk_init()

# Set build type to Debug for debug symbols
set(CMAKE_BUILD_TYPE Debug)

#4add your executable with YOUR  source files 
add_executable(${PROJECT}
     hello_uart.c
)

#5. LINk pico SDK targets (this pulls in SDK  source files !)
target_link_libraries(${PROJECT}
    pico_stdlib   

)

pico_enable_stdio_usb(${PROJECT} 1)
pico_enable_stdio_uart(${PROJECT} 0)
pico_add_extra_outputs(${PROJECT})


