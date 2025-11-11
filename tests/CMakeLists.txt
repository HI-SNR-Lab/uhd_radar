find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(yaml-cpp REQUIRED)
find_package(UHD REQUIRED)

add_executable(test_utils
    sdr/test_utils.cpp
    ../sdr/utils.cpp
)

add_executable(test_sdr
    sdr/test_sdr.cpp
    ../sdr/sdr.cpp
    ../sdr/rf_settings.cpp
)

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

target_compile_definitions(test_sdr PRIVATE CONFIG_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../config")
target_include_directories(test_sdr PRIVATE ../sdr)
target_link_libraries(test_sdr
    uhd
    gtest_main
    Boost::filesystem
    yaml-cpp
)

include(GoogleTest)
gtest_discover_tests(test_utils)
gtest_discover_tests(test_pseudorandom_phase)
gtest_discover_tests(test_sdr)
gtest_discover_tests(test_chirp)
