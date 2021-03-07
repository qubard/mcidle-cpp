cmake_minimum_required(VERSION 3.10)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ".")

find_package(Boost 1.72.0 COMPONENTS filesystem system unit_test_framework REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS})

add_executable(mcidle-varint-test ${PROJECT_SOURCE_DIR}/tests/VarIntTest.cpp)
target_link_libraries(mcidle-varint-test ${Boost_LIBRARIES} zlib mcidlelib ${CURL_LIBRARIES})

add_executable(mcidle-type-test ${PROJECT_SOURCE_DIR}/tests/TypeTest.cpp)
target_link_libraries(mcidle-type-test ${Boost_LIBRARIES} zlib mcidlelib ${CURL_LIBRARIES})

add_executable(mcidle-varlong-test ${PROJECT_SOURCE_DIR}/tests/VarLongTest.cpp)
target_link_libraries(mcidle-varlong-test ${Boost_LIBRARIES} zlib mcidlelib ${CURL_LIBRARIES})

add_test(VarIntTest mcidle-varint-test)
add_test(TypeTest mcidle-type-test)
add_test(VarLongTest mcidle-varlong-test)
