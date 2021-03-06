cmake_minimum_required(VERSION 3.16)
project(filemanager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

option(WARNINGS_AS_ERRORS   "Treat compiler warnings as errors."    OFF)
option(ENABLE_CONAN         "Use Conan as a package manager."       OFF)
option(ENABLE_PVS_STUDIO    "Use PVS-Studio static code analyzer."  OFF) # Option for the local usage only. PVS-Studio isn't installed on GitHub machines.
option(ENABLE_SANITIZERS    "Use sanitizers to detect errors."      OFF) # Option for the test builds. Do not use it in the production builds.

set(TARGET_NAME filemanager)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif ()

# Build release version if not specified otherwise.
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()
#
find_package(ICU REQUIRED COMPONENTS uc i18n)
find_package(Boost REQUIRED COMPONENTS iostreams filesystem locale system)
find_package(LibArchive REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Looks like it is not necessary:
set(Boost_USE_STATIC_LIBS OFF) # enable dynamic linking
set(Boost_USE_MULTITHREAD ON)  # enable multithreading

include_directories(inc)
include_directories(${LIBARCHIVE_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIR} ${ICU_INCLUDE_DIRS})



add_executable(${TARGET_NAME} ${PROJECT_RESOURCE} main.cpp ${SRC} src/filemanager.cpp src/filemanager.ui src/archiving.cpp inc/filemanager.h inc/archiving.h)

add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)

target_link_libraries(filemanager LINK_PUBLIC ${Boost_LIBRARIES} ${ICU_LIBRARIES} )
target_link_libraries(filemanager LINK_PUBLIC ${LibArchive_LIBRARIES})
target_link_libraries(filemanager LINK_PUBLIC Qt5::Widgets)

set(LibZip_LIBRARIES "")
include(CheckLibraryExists)
SET(LibZip_LIBRARY_PATH "")
check_library_exists(zip zip_open_from_source
        "${LibZip_LIBRARY_PATH}" LibZip_FOUND)
if(LibZip_FOUND)
    message("Found LibZip")
    list(APPEND LibZip_LIBRARIES "zip" )
    link_directories(${LibZip_LIBRARY_PATH})
    target_link_libraries(filemanager LINK_PUBLIC ${LibZip_LIBRARIES})
endif()
