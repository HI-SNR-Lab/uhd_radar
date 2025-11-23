find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(yaml-cpp CONFIG REQUIRED)

add_executable(test_utils
    sdr/test_utils.cpp
    ../sdr/utils.cpp
)

add_executable(test_sdr
    sdr/test_sdr.cpp
    ../sdr/sdr.cpp
)

if (RADAR_MODE STREQUAL "uhd")
    find_package(UHD REQUIRED)
    add_executable(test_hisnr_usrp
        sdr/test_hisnr_usrp.cpp
        ../sdr/sdr.cpp
        ../sdr/hisnr_usrp.cpp
        ../sdr/usrp_rf_settings.cpp
    )
elseif (RADAR_MODE STREQUAL "pynq")
    add_executable(test_hisnr_rfsoc42
        sdr/test_hisnr_rfsoc42.cpp
        ../sdr/sdr.cpp
        ../sdr/hisnr_rfsoc42.cpp
    )
else()
    message(FATAL_ERROR "Unknown RADAR_MODE: must be 'uhd' or 'pynq'")
endif()

add_executable(test_chirp
    sdr/test_chirp.cpp
    ../sdr/chirp.cpp
)

add_executable(test_pseudorandom_phase
    sdr/test_pseudorandom_phase.cpp
    ../sdr/pseudorandom_phase.cpp
)

target_include_directories(test_utils PRIVATE ../sdr)
target_link_libraries(test_utils
    gtest_main
    Boost::filesystem
)

target_include_directories(test_pseudorandom_phase PRIVATE ../sdr)
target_link_libraries(test_pseudorandom_phase
    gtest_main
    Boost::filesystem
)

target_compile_definitions(test_chirp PRIVATE CONFIG_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../config")
target_include_directories(test_chirp PRIVATE ../sdr)
target_link_libraries(test_chirp
    gtest_main
    Boost::filesystem
    yaml-cpp
)

if(RADAR_MODE STREQUAL "uhd")
    target_compile_definitions(test_sdr PRIVATE CONFIG_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../config")
    target_include_directories(test_sdr PRIVATE ../sdr)
    target_link_libraries(test_sdr
        gtest_main
        Boost::filesystem
        yaml-cpp
    )

    target_compile_definitions(test_hisnr_usrp PRIVATE CONFIG_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../config")
    target_include_directories(test_hisnr_usrp PRIVATE ../hisnr_usrp)
    target_link_libraries(test_hisnr_usrp
        ${UHD_LIBRARIES}
        gtest_main
        Boost::filesystem
        yaml-cpp
    )
elseif(RADAR_MODE STREQUAL "pynq")
    target_compile_definitions(test_sdr PRIVATE CONFIG_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../config")
    target_include_directories(test_sdr PRIVATE ../sdr)
    target_link_libraries(test_sdr
        gtest_main
        Boost::filesystem
        yaml-cpp
    )

    target_compile_definitions(test_hisnr_rfsoc42 PRIVATE CONFIG_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../config")
    target_include_directories(test_hisnr_rfsoc42 PRIVATE ../hisnr_rfsoc42)
    target_link_libraries(test_hisnr_rfsoc42
        gtest_main
        Boost::filesystem
        yaml-cpp
        ${pybind11_LIBRARIES}
        ${Python3_LIBRARIES}
    )
else()
    message(FATAL_ERROR "Unknown RADAR_MODE: must be 'uhd' or 'pynq'")
endif()

include(GoogleTest)
gtest_discover_tests(test_utils)
gtest_discover_tests(test_pseudorandom_phase)
gtest_discover_tests(test_sdr)

if(RADAR_MODE STREQUAL "uhd")
    gtest_discover_tests(test_hisnr_usrp)
elseif(RADAR_MODE STREQUAL "pynq")
    gtest_discover_tests(test_hisnr_rfsoc42)
else()
    message(FATAL_ERROR "Unknown RADAR_MODE: must be 'uhd' or 'pynq'")
endif()

gtest_discover_tests(test_chirp)
