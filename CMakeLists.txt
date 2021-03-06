#
# Copyright (c) 2019, András Bodor
# Licensed under the BSD 3-Clause license
# For more information see the supplied
# LICENSE file
#
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
project(InfoParse CXX)

set(InfoParse_VERSION_MAJOR 2)
set(InfoParse_VERSION_MINOR 1)
set(InfoParse_VERSION_PATCH 0)
set(InfoParse_VERSION "${InfoParse_VERSION_MAJOR}.${InfoParse_VERSION_MINOR}.${InfoParse_VERSION_PATCH}")

string(TIMESTAMP InfoParse_VERSION_BUILD_DATE "%Y-%m-%d at %H:%M:%S (%s)" UTC)

configure_file( # For version
        "${PROJECT_SOURCE_DIR}/include/info/parse/versioning.hpp.in"
        "${PROJECT_SOURCE_DIR}/include/info/parse/versioning.hpp"
        )
set_property(GLOBAL PROPERTY INFO_PARSE_INCLUDE "${PROJECT_SOURCE_DIR}/include")

configure_file( # For version
        "${PROJECT_SOURCE_DIR}/dox_site/installation.md.in"
        "${PROJECT_SOURCE_DIR}/dox_site/installation.md"
        )

if (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
    message(WARNING "MSVC is most likely going to fail building this, at least for me it is unable to do so.")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif ()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(USE_CLANG_PARSING "YES")
    set(COMPILATION_DB_PATH ${PROJECT_BINARY_DIR}/compile_command.json)

    message(STATUS "Searching for lld")
    find_program(USABLE_LLD lld DOC "Search for llvm's lld linker")
    if (NOT ${USABLE_LLD-NOTFOUND})
        message(STATUS "Searching for lld - Success")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=lld")
    else ()
        message(STATUS "Searching for lld - Failure")
    endif ()

else ()
    set(USE_CLANG_PARSING "NO")
    set(COMPILATION_DB_PATH "")
endif ()

configure_file( # For clang support
        ${PROJECT_SOURCE_DIR}/.doxygen.in
        ${PROJECT_SOURCE_DIR}/.doxygen
        @ONLY
        )

message(STATUS "Looking for a presinstalled Boost")
find_package(Boost COMPONENTS unit_test_framework)
if (Boost_FOUND)
    set(IP_USE_BOOST 1)
    message(STATUS "Looking for a presinstalled Boost - Success")
else ()
    set(IP_USE_BOOST 0)
    message(STATUS "Looking for a presinstalled Boost - Failure")
    include(utility/mkboost.cmake)
endif ()

configure_file( # For Boost support
        ${PROJECT_SOURCE_DIR}/include/info/parse/config.hpp.in
        ${PROJECT_SOURCE_DIR}/include/info/parse/config.hpp
        )

set(InfoParse_SOURCES
    src/versioning.cpp src/utils.cpp
    src/Option_.cpp
    src/OptionHandler_.cpp
    src/OptionString.cpp
    src/OptionsParser.cpp
    src/Lazy.cpp
    )

set(InfoParse_HEADERS
    # Config'd
    include/info/parse/versioning.hpp
    include/info/parse/config.hpp
    # Procedural headers
    include/info/parse/utils.hpp
    # Classes
    include/info/parse/Option_.hpp
    include/info/parse/OptionHandler_.hpp
    include/info/parse/OptionsParser.hpp
    include/info/parse/OptionString.hpp
    include/info/parse/Lazy.hpp
    )

add_library(infoparse SHARED ${InfoParse_HEADERS} ${InfoParse_SOURCES})
set_target_properties(infoparse PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(test)

set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
if (WIN32 AND NOT CYGWIN)
    set(DEF_INSTALL_CMAKE_DIR CMake)
else ()
    set(DEF_INSTALL_CMAKE_DIR lib/CMake/InfoParse)
endif ()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")

foreach (header ${InfoParse_HEADERS})
    # Dynamic library
    get_target_property(pub infoparse PUBLIC_HEADER)
    if ("${pub}" STREQUAL "pub-NOTFOUND")
        set_target_properties(infoparse PROPERTIES PUBLIC_HEADER "${header}")
    else ()
        set_target_properties(infoparse PROPERTIES PUBLIC_HEADER "${header};${pub}")
    endif ()
endforeach ()

install(TARGETS infoparse EXPORT InfoParseConfig
        RUNTIME DESTINATION ${INSTALL_BIN_DIR}
        LIBRARY DESTINATION ${INSTALL_LIB_DIR}
        ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
        PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDE_DIR}
        )
install(EXPORT InfoParseConfig DESTINATION ${INSTALL_CMAKE_DIR})





