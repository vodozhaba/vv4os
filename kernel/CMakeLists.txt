cmake_minimum_required(VERSION 3.1)

cmake_policy(VERSION 3.1)

project(vv4os_kernel C)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(KERNEL "kernel.elf")
set(LD_SCRIPT "linker.ld")
set(C_FLAGS "-Wall -Wextra -Werror -ffreestanding -std=c11 -I${SRC_DIR}")
set(LINKER_FLAGS "-ffreestanding -nostdlib -lgcc -T${SRC_DIR}/${LD_SCRIPT}")

file(GLOB_RECURSE SRC_ASM
	RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
	"${SRC_DIR}/*.asm")

file(GLOB_RECURSE SRC_C
	RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
	"${SRC_DIR}/*.c")

file(GLOB_RECURSE SRC_H
	RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
	"${SRC_DIR}/*.h")

if(CMAKE_SYSTEM_PROCESSOR MATCHES "i.86")
	add_definitions(-D__X86__)
	set(CMAKE_ASM_NASM_OBJECT_FORMAT "elf")
endif()

enable_language(ASM_NASM)
if(NOT CMAKE_ASM_NASM_COMPILER_LOADED)
	message(FATAL_ERROR "NASM has not been loaded. Abort.")
endif()

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()
if(CMAKE_BUILD_TYPE STREQUAL Release)
	add_definitions(-DRELEASE)
elseif(CMAKE_BUILD_TYPE STREQUAL Debug)
	add_definitions(-DDEBUG)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINKER_FLAGS}")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "") # work around -rdynamic

add_executable(${KERNEL} ${SRC_ASM} ${SRC_C} ${SRC_H})
