cmake_minimum_required(VERSION 3.21.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

project(program LANGUAGES C CXX ASM)

set(CPUFLAGS -mcpu=cortex-m3 -mfpu=vfp)
set(LINKER_SCRIPT linker/stm32f401xc.ld)

add_executable(program.elf
    main.c

    gpio-v2.c
    systick.c
    syscalls.c

    vectors/stm32f401.s
    startup.s
)

target_include_directories(program.elf PUBLIC
    .
    vendor/CMSIS_5/CMSIS/Core/Include
    vendor/cmsis_device_f4/Include
)

target_link_directories(program.elf PUBLIC
    .
)

set_property(TARGET program.elf PROPERTY C_STANDARD 17)
set_property(TARGET program.elf PROPERTY CXX_STANDARD 17)
set_property(TARGET program.elf PROPERTY LINK_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_SCRIPT})

target_compile_options(program.elf PUBLIC
    ${CPUFLAGS}

    -Og

    -Wall -Werror

    -g3 -gz
    -fno-eliminate-unused-debug-types
    -fno-eliminate-unused-debug-symbols
)

target_link_options(program.elf PUBLIC
    ${CPUFLAGS}
    --specs=nano.specs
    --specs=nosys.specs

    -e Reset_Handler
    LINKER:-T,${LINKER_SCRIPT}
    LINKER:--nmagic
    LINKER:--compress-debug-sections=zlib
    LINKER:--no-warn-rwx-segments # cf. https://github.com/raspberrypi/pico-sdk/issues/1029
)
