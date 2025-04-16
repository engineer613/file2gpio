cmake_minimum_required(VERSION 3.7)

set(CMAKE_TOOLCHAIN_FILE "bbb-toolchain.cmake")

# set the project name and version
project(File2GPIO LANGUAGES CXX C VERSION 1.0)
set(TARGET_NAME ${CMAKE_PROJECT_NAME})

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Threads REQUIRED)

# BBB GPIO Pin Library
add_library(BBBGpioPin STATIC src/BeagleBoneBlack_GPIO/BBBGpioPin.cpp src/BeagleBoneBlack_GPIO/BBBGpioPin.h)

# File Reader Library
add_library(FileReader STATIC src/FileReader/FileReader.cpp)
target_include_directories(FileReader PUBLIC
        src/FileReader
        src/PooledQueue
        src/Utils
)

# GPIO Writer
add_library(GpioWriter STATIC src/GpioWriter/GpioWriter.hpp src/GpioWriter/GpioWriter.cpp)
target_include_directories(GpioWriter PUBLIC
        src/GpioWriter
        src/PooledQueue
        src/Utils
        src/BeagleBoneBlack_GPIO/
)
target_link_libraries(GpioWriter PRIVATE BBBGpioPin Threads::Threads)

# Application Executable
#CMake 3.7 requires add_executable to be passed atleast one source file rightaway
add_executable(${TARGET_NAME} src/main.cpp )
#target_sources(${TARGET_NAME} PRIVATE main.cpp bbb_gpio_pin.cpp)

target_include_directories(${TARGET_NAME} PRIVATE
        src
        src/BeagleBoneBlack_GPIO
)

target_link_libraries(${TARGET_NAME} PRIVATE BBBGpioPin FileReader GpioWriter Threads::Threads)
