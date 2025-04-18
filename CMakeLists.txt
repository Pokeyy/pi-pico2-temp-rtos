
# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

cmake_minimum_required(VERSION 3.31)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_BOARD pico2 CACHE STRING "Board type") # Change before CACHE STRING or change bottom right of board:

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(pi-pico2-temp-rtos C CXX ASM) # defines project name with languages

# Initialize FreeRTOS Kernel
set(FREERTOS_KERNEL_PATH ${PROJECT_SOURCE_DIR}/lib/FreeRTOS-Kernel)         # Kernel path that allows .cmake file to handle everything for the kernel
include(cmake/FreeRTOS_Kernel_import.cmake)                                 # kernel handling provided by FreeRTOS


# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(${PROJECT_NAME}
    src/main.cpp
    #src/rtos-1C-blinky.cpp
    #src/pi-pico-rtos-ex.cpp
)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/lib/pico_dht)

pico_set_program_name(${PROJECT_NAME} "${PROJECT_NAME}")
pico_set_program_version(${PROJECT_NAME} "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(${PROJECT_NAME} 1)
pico_enable_stdio_usb(${PROJECT_NAME} 1)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/include 
    ${CMAKE_CURRENT_LIST_DIR}/lib/pico_dht/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    pico_async_context_freertos
    FreeRTOS-Kernel-Heap4
    pico_stdlib
    dht
)

if(PICO_CYW43_SUPPORTED)
    # For led support on pico_w
    target_link_libraries(${PROJECT_NAME} PRIVATE
        pico_cyw43_arch_none
    )
endif()

# Add the standard include files to the build
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(${PROJECT_NAME})

