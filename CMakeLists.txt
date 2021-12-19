cmake_minimum_required(VERSION 3.16)
project(parser)

set(CMAKE_CXX_STANDARD 20)
#======LIEF========
include(ExternalProject)


# LIEF as an External Project
# ===========================
set(LIEF_PREFIX       "${CMAKE_CURRENT_BINARY_DIR}/LIEF")
set(LIEF_INSTALL_DIR  "${LIEF_PREFIX}")
set(LIEF_INCLUDE_DIRS "${LIEF_PREFIX}/include")

# LIEF static library
set(LIB_LIEF
        "${LIEF_PREFIX}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}LIEF${CMAKE_STATIC_LIBRARY_SUFFIX}")

# URL of the LIEF repo (Can be your fork)
set(LIEF_GIT_URL "https://github.com/lief-project/LIEF.git")

# LIEF's version to be used (can be 'master')
set(LIEF_VERSION 0.11.0)

# LIEF compilation config
set(LIEF_CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DLIEF_DOC=off
        -DLIEF_PYTHON_API=off
        -DLIEF_EXAMPLES=off
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        )

if(MSVC)
    list(APPEND ${LIEF_CMAKE_ARGS} -DLIEF_USE_CRT_RELEASE=MT)
endif()

ExternalProject_Add(LIEF
        PREFIX           "${LIEF_PREFIX}"
        GIT_REPOSITORY   ${LIEF_GIT_URL}
        GIT_TAG          ${LIEF_VERSION}
        INSTALL_DIR      ${LIEF_INSTALL_DIR}
        CMAKE_ARGS       ${LIEF_CMAKE_ARGS}
        BUILD_BYPRODUCTS ${LIEF_LIBRARIES}
        UPDATE_COMMAND   ""
        )
#=======END LIEF INIT=========


add_executable(parser
            main.cpp
            include/Unzipper.h
            src/Unzipper.cpp
            src/FileBundleParser.cpp include/FileBundleParser.h
            src/Dispatcher.cpp include/Dispatcher.h
            include/File.h
        	src/PlistFile.cpp include/PlistFile.h
            src/MediaFile.cpp include/MediaFile.h
            src/PlistParser.cpp include/PlistParser.h
            pugixml-1.11/src/pugixml.hpp
            pugixml-1.11/src/pugixml.cpp
            include/base64.h
            src/PlistDate.cpp include/PlistDate.h
            src/ConfigFile.cpp include/ConfigFile.h
            include/PlistEntry.h src/PlistEntry.cpp
            src/BinaryPlistParser.cpp include/BinaryPlistParser.h
            json/json.hpp src/JsonFile.cpp include/JsonFile.h
            src/Executable.cpp include/Executable.h
            src/FileTypeResolver.cpp include/FileTypeResolver.h)

#========SETTING UP LIEF LIB=============
# Setup the LIEF include directory
target_include_directories(parser
        PUBLIC
        ${LIEF_INCLUDE_DIRS}
        )

# Link the executable with LIEF
target_link_libraries(parser PUBLIC ${LIB_LIEF} magic)

add_dependencies(parser LIEF)
