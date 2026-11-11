# iOS CMake Toolchain File
# Designed for cross-compiling OpenBOR for iOS (ARM64)
# Compatible with CMake 3.16+, Xcode generators, and macOS CI runners

cmake_minimum_required(VERSION 3.16)

# Force iOS system name
if(NOT DEFINED CMAKE_SYSTEM_NAME)
    set(CMAKE_SYSTEM_NAME iOS)
endif()

# Minimum iOS deployment version
if(NOT DEFINED CMAKE_OSX_DEPLOYMENT_TARGET)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "Minimum iOS deployment version")
endif()

# Default to arm64 architecture for 64-bit iOS devices
if(NOT DEFINED CMAKE_OSX_ARCHITECTURES)
    set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "Target iOS architectures")
endif()

# SDK Selection
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    if(NOT DEFINED CMAKE_OSX_SYSROOT)
        set(CMAKE_OSX_SYSROOT "iphoneos" CACHE STRING "iOS SDK to use (iphoneos or iphonesimulator)")
    endif()
else()
    if(NOT DEFINED CMAKE_OSX_SYSROOT)
        set(CMAKE_OSX_SYSROOT "iphoneos" CACHE STRING "iOS SDK to use")
    endif()
endif()

# Disable bitcode (deprecated in Xcode 14, rejected in Xcode 15+)
set(CMAKE_XCODE_ATTRIBUTE_ENABLE_BITCODE "NO")

# Code Signing Settings (default to ad-hoc / empty for unsigned development builds)
if(NOT DEFINED CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY)
    set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "" CACHE STRING "Code signing identity")
endif()

set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO")
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "YES")

# Search path constraints for cross-compiling
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

message(STATUS "iOS Toolchain Configuration:")
message(STATUS "  - System Name:        ${CMAKE_SYSTEM_NAME}")
message(STATUS "  - Deployment Target:  ${CMAKE_OSX_DEPLOYMENT_TARGET}")
message(STATUS "  - Architectures:      ${CMAKE_OSX_ARCHITECTURES}")
message(STATUS "  - SDK:                ${CMAKE_OSX_SYSROOT}")
