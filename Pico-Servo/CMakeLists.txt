cmake_minimum_required(VERSION 3.12)

# Set the path to the Pico SDK
set(PICO_SDK_PATH "/Users/charlie/Desktop/pico-sdk-master")

# Pull in SDK (must be before project)
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(Servo)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# Initialize the SDK
pico_sdk_init()

include(example_auto_set_url.cmake)

add_subdirectory(servo)
add_subdirectory(Examples)
