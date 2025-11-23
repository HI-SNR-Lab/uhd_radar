#
# Copyright 2014-2015 Ettus Research LLC
# Copyright 2018 Ettus Research, a National Instruments Company
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

# # for debugging purposes
# set(CMAKE_FIND_DEBUG_MODE ON)
# set(CMAKE_FIND_PACKAGE_TRACE ON)

message(STATUS "We got to here, since ${RADAR_MODE} is uhd")

# Recent update to CMAKE (3.30) that changes how FindBoost library is linked
    # for more details about changes made, run cmake --help CMP0167
cmake_minimum_required(VERSION 3.5.1)
option(RADAR_MODE "What type of software is your board using (uhd/pynq)?: ")
message(STATUS "Building executable for a board running ${RADAR_MODE} software")
# set(CMAKE_LIBRARY_PATH "$ENV{CONDA_PREFIX}/lib")
# set(CMAKE_INCLUDE_PATH "$ENV{CONDA_PREFIX}/include")

####------------ MAKING UHD-BASED RADAR ------------####

message(STATUS "We got to here, since ${RADAR_MODE} is uhd")
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" AND ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_EXE_LINKER_FLAGS "-lthr ${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_CXX_FLAGS "-stdlib=libc++ ${CMAKE_CXX_FLAGS}")
endif()

set(CMAKE_CXX_FLAGS "-Wno-psabi ${CMAKE_CXX_FLAGS}")

### Set up build environment ##################################################
# Choose a static or shared-library build (shared is default, and static will probably need some special care!)
option(UHD_USE_STATIC_LIBS OFF) # Set this to ON in order to link a static build of UHD

# Adds UHD and yaml-cpp as dependencies to this project
find_package(UHD 3.15.0 REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

# Sets components, then includes UHDBoost to do the actual finding
set(UHD_BOOST_REQUIRED_COMPONENTS
    program_options
    system
    thread
    filesystem
    chrono
)
set(BOOST_MIN_VERSION 1.58)
include(UHDBoost)

# Includes all directories required
include_directories(
    ${Boost_INCLUDE_DIRS}
    ${UHD_INCLUDE_DIRS}
    ${YAML_CPP_INCLUDE_DIR}
)
link_directories(${Boost_LIBRARY_DIRS})

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# sources for the executable "radar" #
add_executable(radar main.cpp 
                     ../hisnr_usrp.cpp ../hisnr_usrp.hpp 
                     rf_settings.cpp rf_settings.hpp 
                     ../utils.cpp ../utils.hpp 
                     ../pseudorandom_phase.cpp ../pseudorandom_phase.hpp 
                     ../chirp.hpp ../chirp.cpp 
                     ../sdr.cpp ../sdr.hpp 
                     ../common.hpp
                )

# Psuedorandom phase noise generation for post-processing
add_executable(pseudorandom_phase_codes_to_file ../pseudorandom_phase_to_file.cpp 
                                                ../pseudorandom_phase.cpp ../pseudorandom_phase.hpp 
                                                ../common.hpp
                )

enable_testing()
add_subdirectory(${CMAKE_SOURCE_DIR}/../tests ${CMAKE_BINARY_DIR}/tests)

set(CMAKE_BUILD_TYPE "Release")

# Shared library case: All we need to do is link against the library, and
# anything else we need (in this case, some Boost libraries):
if(NOT UHD_USE_STATIC_LIBS)
    message(STATUS "Linking against shared UHD library.")
    target_link_libraries(radar ${UHD_LIBRARIES} ${Boost_LIBRARIES} yaml-cpp)
# Shared library case: All we need to do is link against the library, and
# anything else we need (in this case, some Boost libraries):
else(NOT UHD_USE_STATIC_LIBS)
    message(STATUS "Linking against static UHD library.")
    target_link_libraries(radar

        # We could use ${UHD_LIBRARIES}, but linking requires some extra flags,
        # so we use this convenience variable provided to us
        ${UHD_STATIC_LIB_LINK_FLAG}

        # Also, when linking statically, we need to pull in all the deps for
        # UHD as well, because the dependencies don't get resolved automatically
        ${UHD_STATIC_LIB_DEPS}
    )
endif(NOT UHD_USE_STATIC_LIBS)

### Once it's built... ########################################################
# Here, you would have commands to install your program.
# We will skip these in this example.