cmake_minimum_required(VERSION 3.10)
project(mcidle)

#set(CMAKE_EXE_LINKER_FLAGS " -static")
#set(LLVM_STATIC_LINK_CXX_STDLIB ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(Boost_USE_STATIC_RUNTIME ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_DETAILED_FAILURE_MSG ON)
set(BOOST_DEBUG OFF)

# Link curl statically
set(BUILD_SHARED_LIBS OFF)
add_definitions(-DCURL_STATICLIB)
add_compile_options(-DCURL_STATICLIB)
set(CURL_STATICLIB ON)

set(CMAKE_CXX_EXTENSIONS OFF)

#set(OPENSSL_USE_STATIC_LIBS TRUE)

# Build zlib
add_subdirectory(zlib)

# Build json and disable tests
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(json)

set(mcidle_include_dirs ${PROJECT_SOURCE_DIR}/mcidle/include)
file(GLOB_RECURSE source_files "mcidle/src/*" "mcidle/include/*")

find_package(Boost 1.72.0 REQUIRED COMPONENTS system thread chrono)
find_package(OpenSSL REQUIRED)

find_package(CURL REQUIRED)

add_library(mcidlelib STATIC ${source_files})

include_directories(${Boost_INCLUDE_DIRS} lib ${mcidle_include_dirs} ${CURL_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})

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

add_executable(mcidle "mcidle/main.cpp")

target_compile_features(mcidlelib PRIVATE cxx_std_17)
target_compile_features(mcidle PRIVATE cxx_std_17)

target_link_libraries(mcidlelib nlohmann_json::nlohmann_json zlib ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} ${CURL_LIBRARIES} -lm -ldl -lpthread)
target_link_libraries(mcidle mcidlelib ${CURL_LIBRARIES} -lm -ldl -lpthread)
target_include_directories(mcidle PUBLIC ${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${CURL_INCLUDE_DIRS})

message(STATUS ${OPENSSL_LIBRARIES})
message(STATUS ${OPENSSL_INCLUDE_DIR})

# Build unit tests
#add_subdirectory(tests)
