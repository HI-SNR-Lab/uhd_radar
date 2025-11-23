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

if(DEFINED ENV{VIRTUAL_ENV})
    message(STATUS "Using venv $ENV{VIRTUAL_ENV} to install Python3")
    set(Python3_EXECUTABLE "$ENV{VIRTUAL_ENV}/bin/python3" CACHE FILEPATH "Python3 interpreter")
endif()

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

include_directories(/usr/local/share/pynq-venv/include)

# Python libs
set(RADAR_LIBS python3.10)

include_directories(
    ${YAML_CPP_INCLUDE_DIR}
    ${pybind11_INCLUDE_DIRS}
    ${Python3_INCLUDE_DIRS}
)

find_package(pybind11 REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

add_executable(radar main.cpp 
                     hisnr_rfsoc42.cpp amd/hisnr_rfsoc42.hpp 
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
target_link_libraries(radar ${pybind11_LIBRARIES} ${Python3_LIBRARIES} yaml-cpp)


### Once it's built... ########################################################
# Here, you would have commands to install your program.
# We will skip these in this example.