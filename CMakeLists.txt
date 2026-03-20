cmake_minimum_required(VERSION 4.0)

project(GryphAutoDownload)
enable_language(CXX)

set(CMAKE_CXX_STANDARD 20)

if(CMAKE_HOST_LINUX)
    add_compile_options(-Wall)
elseif(CMAKE_HOST_WIN32)
    add_compile_options(/W3)
    add_compile_options(/utf-8)
endif()

if(NOT DEFINED MSVC_RUNTIME_DYNAMIC)
    set(MSVC_RUNTIME_DYNAMIC ON)
endif()

if (MSVC_RUNTIME_DYNAMIC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
else()
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(spdlog REQUIRED)
find_package(zlib-ng REQUIRED)
find_package(Boost REQUIRED)

add_executable(hgdler)
target_sources(hgdler
PRIVATE
    src/main.cpp
    src/filedl/filedl.cpp
    src/data_queue/data_queue.cpp
    src/jobs_main/download_main.cpp
    src/md5calc/md5calc.cpp
    src/zipfile/zipfile.cpp
    src/jobs_main/extract_main.cpp
)
target_include_directories(hgdler PRIVATE src)
target_include_directories(hgdler PRIVATE ${CURL_INCLUDE_DIRS})
target_include_directories(hgdler PRIVATE ${OPENSSL_INCLUDE_DIR})
target_include_directories(hgdler PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(hgdler PRIVATE CURL::libcurl)
target_link_libraries(hgdler PRIVATE OpenSSL::SSL)
target_link_libraries(hgdler PRIVATE spdlog::spdlog)
target_link_libraries(hgdler PRIVATE zlib-ng::zlib)
