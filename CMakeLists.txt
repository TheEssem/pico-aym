# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS, ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
# set(PICO_SDK_PATH "/home/esm/pico/pico-sdk")

set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(cmake/pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(pico-aym C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

include(cmake/bin2h.cmake)
#bin2h(SOURCE_FILE STORML~1.YM HEADER_FILE stormlord.h VARIABLE_NAME stormlord)
#bin2h(SOURCE_FILE phallanx.ym HEADER_FILE phallanx.h VARIABLE_NAME phallanx)
bin2h(SOURCE_FILE tunes/STORML~1.YM HEADER_FILE tunes/build/ym-file.h VARIABLE_NAME ym_file)

bin2h(SOURCE_FILE tunes/changsha.vgm HEADER_FILE tunes/build/vgm-file.h VARIABLE_NAME vgm_file)

# Add executable. Default name is the project name, version 0.1

#add_executable(pico-aym pico-aym.c ym-player.c ym-file.h)
add_executable(pico-aym pico-aym.c
          players/vgm-player.c
          tunes/build/vgm-file.h
          aym.c
          usb.c)

# Create C header file with the name <pio program>.pio.h
#pico_generate_pio_header(pico-aym
#        ${CMAKE_CURRENT_LIST_DIR}/clock.pio
#)

pico_set_program_name(pico-aym "pico-aym")
pico_set_program_version(pico-aym "0.1")

#pico_enable_stdio_uart(pico-aym 1)
#pico_enable_stdio_usb(pico-aym 0)

# Add the standard library to the build
target_link_libraries(pico-aym
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(pico-aym PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries(pico-aym 
        #hardware_dma
        #hardware_pio
        hardware_adc
        hardware_pwm
        pico_multicore
        pico_stdlib
        tinyusb_device
        )

pico_add_extra_outputs(pico-aym)

