cmake_minimum_required(VERSION 3.16)

project(OpenBOR
    VERSION 3.0.6391
    DESCRIPTION "OpenBOR v3.0 Build 6391 Game Engine - Native iOS Port"
    LANGUAGES C CXX
)

# Enable Objective-C / Objective-C++ if on Apple platforms
if(APPLE OR CMAKE_SYSTEM_NAME STREQUAL "iOS")
    enable_language(OBJC)
    enable_language(OBJCXX OPTIONAL)
endif()

# Global C/C++ Standards
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Export compile commands for language servers and static analysis
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options
option(OPENBOR_BUILD_IOS "Target native iOS" OFF)
if(CMAKE_SYSTEM_NAME STREQUAL "iOS" OR IOS)
    set(OPENBOR_BUILD_IOS ON CACHE BOOL "Target native iOS" FORCE)
endif()

message(STATUS "----------------------------------------------------")
message(STATUS "OpenBOR v3.0 Build 6391 Build Configuration")
message(STATUS "Target System: ${CMAKE_SYSTEM_NAME} (${CMAKE_SYSTEM_PROCESSOR})")
message(STATUS "iOS Target:    ${OPENBOR_BUILD_IOS}")
message(STATUS "----------------------------------------------------")

add_subdirectory(engine)
