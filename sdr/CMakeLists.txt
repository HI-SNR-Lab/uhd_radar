#
# Copyright 2014-2015 Ettus Research LLC
# Copyright 2018 Ettus Research, a National Instruments Company
#
# SPDX-License-Identifier: GPL-3.0-or-later

# Recent update to CMAKE (3.30) that changes how FindBoost library is linked
    # for more details about changes made, run cmake --help CMP0167
cmake_minimum_required(VERSION 3.30)

# # for debugging purposes
# set(CMAKE_FIND_DEBUG_MODE ON)
# set(CMAKE_FIND_PACKAGE_TRACE ON)

set(ETTUS "ettus")
set(AMD "amd")

# If Conda is running, user is assumed to be using an Ettus radio
if(DEFINED ENV{CONDA_DEFAULT_ENV})
    message(STATUS "Running in Conda environment: $ENV{CONDA_DEFAULT_ENV}")
    set(ASSUMED_RADIO_TYPE ETTUS)
else()
    message(STATUS "Running in PYNQ environment: pynq-venv")
    set(ASSUMED_RADIO_TYPE AMD)
endif()

message(STATUS "assumed radio type: ${${ASSUMED_RADIO_TYPE}}")

# Create Project
if(${${ASSUMED_RADIO_TYPE}} STREQUAL "${ETTUS}")
    project(INIT_USRP CXX)
    message(STATUS "conda prefix: $ENV{CONDA_PREFIX}")
    set(CMAKE_LIBRARY_PATH "$ENV{CONDA_PREFIX}/lib")
    set(CMAKE_INCLUDE_PATH "$ENV{CONDA_PREFIX}/include")
else()
    find_package(Python3 REQUIRED COMPONENTS Interpreter)
    execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "import sysconfig; print(sysconfig.get_paths()['purelib'])"
                            OUTPUT_VARIABLE PYNQ_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(CMAKE_LIBRARY_PATH "$ENV{PYNQ_PATH}")
    set(CMAKE_INCLUDE_PATH "$ENV{PYNQ_PATH}")                 
    project(INIT_RFSOC CXX)
endif()

### Configure Compiler ########################################################
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" AND ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_EXE_LINKER_FLAGS "-lthr ${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_CXX_FLAGS "-stdlib=libc++ ${CMAKE_CXX_FLAGS}")
endif()

 # included to catch errors in compatability between Boost and UHD
add_compile_options(-Wno-enum-constexpr-conversion)

set(CMAKE_CXX_FLAGS "-Wno-psabi ${CMAKE_CXX_FLAGS}")

### Set up build environment ##################################################

## FINDING PACKAGES
if(${${ASSUMED_RADIO_TYPE}} STREQUAL "${ETTUS}")
    # Choose a static or shared-library build (shared is default, and static will
    # probably need some special care!)

    # Set this to ON in order to link a static build of UHD:
    option(UHD_USE_STATIC_LIBS OFF)

    # To add UHD as a dependency to this project, add a line such as this:
    # find_package(UHD 3.15.0 REQUIRED)
    # To specify an exact version: find_package(UHD 4.0.0 EXACT REQUIRED)

    # As specified in environment.yaml, UHD has an open bug 
    # associated with 4.5.0 and above: https://github.com/EttusResearch/uhd/issues/751
    find_package(UHD 3.15.0 REQUIRED)
    if(UHD_VERSION VERSION_GREATER "4.4.0")
        message(FATAL_ERROR "UHD must be below version 4.5.0. Found version ${UHD_VERSION}.")
    endif()

    find_package(yaml-cpp CONFIG REQUIRED)
else()  ## Assumed to be AMD
    find_package(yaml-cpp CONFIG REQUIRED)
    find_package(pybind11 REQUIRED)
    find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
    message("THE PYTHON LIBRARY YOU'RE LINKING IS ${pybind11_INCLUDE_DIRS}")
endif()


# This example also requires Boost.
# Set components here, then include UHDBoost to do the actual finding
set(UHD_BOOST_REQUIRED_COMPONENTS
    program_options
    system
    thread
    filesystem
    chrono
)
set(BOOST_MIN_VERSION 1.58)
include(UHDBoost)


## LINKING DIRECTORIES

include_directories(      # need these include and link directories for the build
    ${Boost_INCLUDE_DIRS}
    ${YAML_CPP_INCLUDE_DIR}
)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

if(${${ASSUMED_RADIO_TYPE}} STREQUAL "${AMD}")
    include_directories(
        ${pybind11_INCLUDE_DIRS}
        ${Python3_INCLUDE_DIRS}
    )
endif()

link_directories(${Boost_LIBRARY_DIRS})


### Make the executables #######################################################

# MAKE RADAR EXECUTABLE "radar"
if(${${ASSUMED_RADIO_TYPE}} STREQUAL "${ETTUS}")
    add_executable(radar main.cpp 
                    hisnr_usrp.cpp hisnr_usrp.hpp 
                    usrp_rf_settings.cpp usrp_rf_settings.hpp 
                    utils.cpp utils.hpp 
                    pseudorandom_phase.cpp pseudorandom_phase.hpp 
                    chirp.hpp chirp.cpp 
                    sdr.cpp sdr.hpp 
                    common.hpp common_usrp.hpp)
    # included in this way due to some issues with UHD 
    target_include_directories(radar SYSTEM PRIVATE ${UHD_INCLUDE_DIRS} )
    target_compile_definitions(radar PRIVATE RADIO_ETTUS)
else()
    add_executable(radar main.cpp 
                    hisnr_rfsoc42.cpp hisnr_rfsoc42.hpp 
                    usrp_rf_settings.cpp usrp_rf_settings.hpp 
                    utils.cpp utils.hpp 
                    pseudorandom_phase.cpp pseudorandom_phase.hpp 
                    chirp.hpp chirp.cpp 
                    sdr.cpp sdr.hpp 
                    common.hpp common_rfsoc42.hpp)
    target_compile_definitions(radar PRIVATE RADIO_AMD)
endif(${${ASSUMED_RADIO_TYPE}} STREQUAL "${ETTUS}")

# Psuedorandom phase noise generation for post-processing
if(${${ASSUMED_RADIO_TYPE}} STREQUAL "${ETTUS}")
    add_executable(pseudorandom_phase_codes_to_file pseudorandom_phase_to_file.cpp 
                    pseudorandom_phase.cpp pseudorandom_phase.hpp 
                    common.hpp common_usrp.hpp)
    # included in this way due to some issues with UHD 
    target_include_directories(radar SYSTEM PRIVATE ${UHD_INCLUDE_DIRS} )
else()
    add_executable(pseudorandom_phase_codes_to_file pseudorandom_phase_to_file.cpp 
                    pseudorandom_phase.cpp pseudorandom_phase.hpp 
                    common.hpp common_rfsoc42.hpp)
endif(${${ASSUMED_RADIO_TYPE}} STREQUAL "${ETTUS}")


enable_testing()
add_subdirectory(${CMAKE_SOURCE_DIR}/../tests ${CMAKE_BINARY_DIR}/tests)

set(CMAKE_BUILD_TYPE "Release")


# LINK "radar" TO REQUISITELIBRARIES
if(${${ASSUMED_RADIO_TYPE}} STREQUAL "${ETTUS}")
    # Shared library case: Link against library and some Boost libraries:
    if(NOT UHD_USE_STATIC_LIBS)
        message(STATUS "Linking against shared UHD library.")
        target_link_libraries(radar 
                                ${UHD_LIBRARIES} 
                                ${Boost_LIBRARIES}
                                yaml-cpp
        )

    # Shared library case: Link against the library and some Boost libraries:
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
else()
    target_link_libraries(radar ${Boost_LIBRARIES} 
                                ${pybind11_LIBRARIES} 
                                ${Python3_LIBRARIES} 
                                yaml-cpp
    )
endif(${${ASSUMED_RADIO_TYPE}} STREQUAL "${ETTUS}")

### Once it's built... ########################################################
# Here, you would have commands to install your program.
# We will skip these in this example.