cmake_minimum_required(VERSION 3.15)
project(mcidle)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(Boost_USE_MULTITHREADED ON)
set(Boost_DETAILED_FAILURE_MSG ON)
set(BOOST_DEBUG OFF)

set(CMAKE_CXX_STANDARD 17) # Use C++17

set(OPENSSL_USE_STATIC_LIBS TRUE)

# Build zlib
add_subdirectory(zlib)

# Build json and disable tests
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(json)

set(mcidle_include_dirs ${PROJECT_SOURCE_DIR}/mcidle/include)
file(GLOB_RECURSE source_files "mcidle/src/*" "mcidle/include/*")

add_library(mcidlelib STATIC ${source_files})

find_package(Boost 1.71.0 REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} lib ${mcidle_include_dirs} ${CURL_INCLUDE_DIRS})

if (BOOST_FOUND)
	message("Boost found!")
else()
	message("Boost not found!")
endif()

if (OPENSSL_FOUND)
	message("OpenSSL found!")
else()
	message("OpenSSL not found!")
endif()

if (CURL_FOUND)
	message("Curl found!")
else()
	message("Curl not found!")
endif()

add_executable(mcidle "mcidle/main.cpp")
target_link_libraries(mcidlelib nlohmann_json::nlohmann_json)
target_link_libraries(mcidle ${Boost_LIBRARIES} zlib mcidlelib ${OPENSSL_LIBRARIES} ${CURL_LIBRARIES} nlohmann_json::nlohmann_json)
target_include_directories(mcidle PUBLIC ${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${CURL_INCLUDE_DIRS})

message(STATUS ${OPENSSL_LIBRARIES})
message(STATUS ${OPENSSL_INCLUDE_DIR})

# Build unit tests
add_subdirectory(tests)