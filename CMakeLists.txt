##
## Copyright (C) 2015 DAI Labor, TU Berlin
##

cmake_minimum_required(VERSION 2.8.10 FATAL_ERROR)
set(OPTIMIZE_FOR_TARGET ATOM)

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
project(whistle_detector)
enable_language(CXX)

find_package(qibuild)

include(CheckCXXCompilerFlag)
message("use toolchain: " ${CMAKE_TOOLCHAIN_FILE})
find_package(BOOST REQUIRED COMPONENTS thread)
find_package(FFTW3F REQUIRED)
find_package(ALSA REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
add_definitions(-Wall -Wextra -Werror)
add_definitions(-Wfatal-errors)  # stop after first error

function(add_cxx_compiler_flag_if_possible flag result)
    check_cxx_compiler_flag(${flag} ${result})
    if(${result})
        add_definitions(${flag})
    endif(${result})
endfunction(add_cxx_compiler_flag_if_possible)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # we have to ignore some compiler flags in clang
    add_definitions(-Qunused-arguments)
    if (APPLE)
        # gnu++0x doesn't work with Clang in MacOS
        add_definitions(-std=c++0x -stdlib=libc++)
    endif(APPLE)
endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

# enable SSSE3 optimization when it is possible (Intel Compiler)
add_cxx_compiler_flag_if_possible(-xSSSE3_ATOM SUPPORT_SSSE3_ATOM)
add_cxx_compiler_flag_if_possible(-ipo SUPPORT_IPO)
add_cxx_compiler_flag_if_possible(-fcolor-diagnostics SUPPORT_FCOLOR_DIAGNOSTICS)
add_cxx_compiler_flag_if_possible(-Wno-unused-parameter NO_UNSED_PARAMETER)
add_cxx_compiler_flag_if_possible(-Wno-ignored-qualifiers NO_IGNORED_QUALIFIERS)
add_definitions(-DBOOST_SIGNALS_NO_DEPRECATION_WARNING)  #ignore warnings from boost
add_definitions(-DNAO)


set(SRCS
    src/ALSARecorder.cpp
    src/STFT.cpp
    src/main.cpp
    )
  
include_directories(SYSTEM ${BOOST_INCLUDE_DIRS} ${FFTW3F_INCLUDE_DIRS} ${ALSA_INCLUDE_DIRS} ${ALCOMMON_INCLUDE_DIRS})

option(MODULE_IS_REMOTE "module is compiled as a remote module" OFF)

qi_create_bin(whistle_detector_test ${SRCS} src/test.cpp)
target_link_libraries(whistle_detector_test ${FFTW3F_LIBRARIES} ${ALSA_LIBRARIES})
qi_use_lib(whistle_detector_test PTHREAD)

if(MODULE_IS_REMOTE)
  add_definitions(-DMODULE_IS_REMOTE)
  qi_create_bin(whistle_detector ${SRCS} src/module.cpp)
else(MODULE_IS_REMOTE)
  qi_create_lib(whistle_detector SHARED ${SRCS} src/module.cpp)
endif(MODULE_IS_REMOTE)
target_link_libraries(whistle_detector ${FFTW3F_LIBRARIES} ${ALSA_LIBRARIES})
qi_use_lib(whistle_detector ALCOMMON)

## ignore warnings from naoqi's header files
string(REPLACE ";" " " NAOQI_INCLUDE ${ALCOMMON_INCLUDE_DIRS})
set_target_properties(whistle_detector PROPERTIES APPEND_STRING PROPERTY
    COMPILE_FLAGS "${CMAKE_INCLUDE_SYSTEM_FLAG_CXX} ${NAOQI_INCLUDE}")
