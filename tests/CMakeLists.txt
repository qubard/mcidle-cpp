cmake_minimum_required(VERSION 3.10)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ".")

find_package(Boost 1.72.0 COMPONENTS filesystem system unit_test_framework REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS})

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/tests/*.cpp)

add_executable(mcidle-tests ${TEST_SRC_FILES})
target_link_libraries(mcidle-tests ${Boost_LIBRARIES} zlib mcidlelib ${CURL_LIBRARIES})

enable_testing()
add_test(UnitTests mcidle-tests)
