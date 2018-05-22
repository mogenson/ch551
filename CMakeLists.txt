cmake_minimum_required(VERSION 3.0)

#### setable variables ####
set(SDCC_EXECUTABLE "/usr/bin/sdcc" CACHE PATH "path to sdcc executable" FORCE)
set(SDCC_LIB "/usr/share/sdcc" CACHE PATH "path to sdcc library directory" FORCE)
set(SDCC_FLAGS "-V -mmcs51 --verbose --Werror --std-c11 --model-small" CACHE STRING "preprocessor and linker flags" FORCE)
set(XRAM_LOC "0x0000" CACHE STRING "external ram start location" FORCE)
set(XRAM_SIZE "0x0400" CACHE STRING "external ram size" FORCE)
set(CODE_LOC "0x0000" CACHE STRING "code segment location" FORCE)
set(CODE_SIZE "0x2800" CACHE STRING "code segment size" FORCE)

############# toolchain info, must come before project definition ##############

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER ${SDCC_EXECUTABLE})
set(CMAKE_FIND_ROOT_PATH ${SDCC_LIB})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".lib")
set(CMAKE_SHARED_LIBRARY_PREFIX "")         # lib
set(CMAKE_SHARED_LIBRARY_SUFFIX ".lib")     # .so
set(CMAKE_IMPORT_LIBRARY_PREFIX )
set(CMAKE_IMPORT_LIBRARY_SUFFIX )
set(CMAKE_EXECUTABLE_SUFFIX ".ihx")         # intel hex file
set(CMAKE_LINK_LIBRARY_SUFFIX ".lib")
set(CMAKE_DL_LIBS "")

set(CMAKE_C_OUTPUT_EXTENSION ".rel")

# find sdcclib as CMAKE_AR
# since cmake may already have searched for "ar", sdcclib has to
# be searched with a different variable name (SDCCLIB_EXECUTABLE)
# and must then be forced into the cache
get_filename_component(SDCC_LOCATION "${CMAKE_C_COMPILER}" PATH)
find_program(SDCCLIB_EXECUTABLE sdcclib PATHS "${SDCC_LOCATION}" NO_DEFAULT_PATH)
find_program(SDCCLIB_EXECUTABLE sdcclib)
set(CMAKE_AR "${SDCCLIB_EXECUTABLE}" CACHE FILEPATH "The sdcc librarian" FORCE)

# preprocessor and linker flags
set(CMAKE_C_FLAGS_INIT "-V -mmcs51 --verbose --Werror --std-c11 --model-small --xram-size 0x0400 --xram-loc 0x0000 --code-size 0x2800")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-V -mmcs51 --verbose --Werror --std-c11 --model-small --xram-size 0x0400 --xram-loc 0x0000 --code-size 0x2800")

# compile a C file into an object file
set(CMAKE_C_COMPILE_OBJECT  "<CMAKE_C_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>")

# link object files to an executable
set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_C_COMPILER> <FLAGS> <OBJECTS> -o <TARGET> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <LINK_LIBRARIES>")

# needs sdcc 2.7.0 + sddclib from cvs
set(CMAKE_C_CREATE_STATIC_LIBRARY
      "\"${CMAKE_COMMAND}\" -E remove <TARGET>"
      "<CMAKE_AR> -a <TARGET> <LINK_FLAGS> <OBJECTS> ")

# not supported by sdcc
set(CMAKE_C_CREATE_SHARED_LIBRARY "")
set(CMAKE_C_CREATE_MODULE_LIBRARY "")

############################## project definition ##############################

project(ch551_blinky C) # C only
add_executable(ch551_blinky main.c)

# generate a bin file after build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND sdobjcopy -I ihex -O binary ${PROJECT_NAME}.ihx ${PROJECT_NAME}.bin
    COMMENT "Creating binary output ${PROJECT_NAME}.bin")

# remove additional files with 'make clean'
set(outputs
    ${PROJECT_NAME}.bin
    ${PROJECT_NAME}.lk
    ${PROJECT_NAME}.map
    ${PROJECT_NAME}.mem)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${outputs}")

# flash command
add_custom_target(flash DEPENDS ${PROJECT_NAME}
    COMMAND wchisptool -f ${PROJECT_NAME}.bin -g
    COMMENT "Flashing ${PROJECT_NAME}.bin to CH551 using wchisptool")

# erase command
add_custom_target(erase
    COMMAND wchisptool -e
    COMMENT "Erasing CH551 using wchisptool")
