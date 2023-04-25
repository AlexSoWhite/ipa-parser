cmake_minimum_required(VERSION 3.16)
project(parser)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -O2)

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
            src/FileTypeResolver.cpp include/FileTypeResolver.h include/Logger.h include/checks/OwaspCheck.h src/checks/PermissionCheck.cpp include/checks/PermissionCheck.h include/utils.h src/utils.cpp src/checks/MD5UsageCheck.cpp include/checks/MD5UsageCheck.h src/checks/HttpUsageCheck.cpp include/checks/HttpUsageCheck.h)

#add_library(SwiftDecompiler SwiftDecompiler/main.cpp
#		SwiftDecompiler/swift
#		SwiftDecompiler/BinaryReader/BinaryReader.cpp
#		SwiftDecompiler/BinaryReader/BinaryReader.h
#		SwiftDecompiler/Binary/Binary.cpp
#		SwiftDecompiler/Binary/Binary.h SwiftDecompiler/SwiftDecompiler.h)
#target_compile_features(SwiftDecompiler PRIVATE cxx_std_14)
#find_package(LLVM REQUIRED CONFIG)
#target_link_libraries(SwiftDecompiler PUBLIC iblessing-core SwiftDemangle LLVMSupport)

#add_subdirectory(SwiftDecompiler)
#add_dependencies(parser SwiftDecompiler)
target_link_libraries(parser PUBLIC LIEF magic SwiftDecompiler)
#target_link_libraries(parser SHARED SwiftDecompiler/libSwiftDecompiler.so)
