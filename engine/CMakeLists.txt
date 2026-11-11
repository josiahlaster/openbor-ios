cmake_minimum_required(VERSION 3.16)

# Set executable target
set(TARGET_NAME "OpenBOR")

# -----------------------------------------------------------------------------
# Platform Detection & Flags
# -----------------------------------------------------------------------------
if(CMAKE_SYSTEM_NAME STREQUAL "iOS" OR IOS OR OPENBOR_BUILD_IOS)
    set(OPENBOR_PLATFORM "IOS")
    message(STATUS "Target Platform: iOS (ARM64)")
elseif(WIN32)
    set(OPENBOR_PLATFORM "WIN")
    message(STATUS "Target Platform: Windows")
elseif(APPLE)
    set(OPENBOR_PLATFORM "DARWIN")
    message(STATUS "Target Platform: macOS (Desktop)")
else()
    set(OPENBOR_PLATFORM "LINUX")
    message(STATUS "Target Platform: Linux")
endif()

# -----------------------------------------------------------------------------
# Source File Lists
# -----------------------------------------------------------------------------
set(CORE_SOURCES
    openbor.c
    openborscript.c
    source/stringptr.c
    source/utils.c
    source/stristr.c
    source/omath.c
)

set(ADPCM_SOURCES
    source/adpcmlib/adpcm.c
)

set(GAMELIB_SOURCES
    source/gamelib/anigif.c
    source/gamelib/bitmap.c
    source/gamelib/draw.c
    source/gamelib/draw16.c
    source/gamelib/draw32.c
    source/gamelib/filecache.c
    source/gamelib/filters.c
    source/gamelib/font.c
    source/gamelib/loadimg.c
    source/gamelib/models.c
    source/gamelib/packfile.c
    source/gamelib/palette.c
    source/gamelib/pixelformat.c
    source/gamelib/screen.c
    source/gamelib/screen16.c
    source/gamelib/screen32.c
    source/gamelib/soundmix.c
    source/gamelib/sprite.c
    source/gamelib/spriteq.c
    source/gamelib/spritex8p16.c
    source/gamelib/spritex8p32.c
    source/gamelib/transform.c
    source/gamelib/translation.c
)

set(GFXLIB_SOURCES
    source/gfxlib/2xSaI.c
    source/gfxlib/bilinear.c
    source/gfxlib/dotmatrix.c
    source/gfxlib/gfx.c
    source/gfxlib/hq2x.c
    source/gfxlib/scale2x.c
    source/gfxlib/scanline.c
    source/gfxlib/simple2x.c
    source/gfxlib/tv2x.c
)

set(OPENBORSCRIPT_SOURCES
    source/openborscript/commands.c
    source/openborscript/animation.c
    source/openborscript/axis.c
    source/openborscript/binding.c
    source/openborscript/collision_attack.c
    source/openborscript/collision_body.c
    source/openborscript/collision_entity.c
    source/openborscript/constants.c
    source/openborscript/energy_status.c
    source/openborscript/entity.c
    source/openborscript/index.c
    source/openborscript/layer.c
    source/openborscript/level.c
    source/openborscript/math.c
    source/openborscript/string.c
)

set(PNGLIB_SOURCES
    source/pnglib/pngdec.c
    source/pnglib/savepng.c
)

set(PREPROCESSOR_SOURCES
    source/preprocessorlib/pp_expr.c
    source/preprocessorlib/pp_lexer.c
    source/preprocessorlib/pp_parser.c
)

set(RAMLIB_SOURCES
    source/ramlib/ram.c
)

set(RANDLIB_SOURCES
    source/randlib/rand32.c
)

set(SCRIPTLIB_SOURCES
    source/scriptlib/Instruction.c
    source/scriptlib/Interpreter.c
    source/scriptlib/Lexer.c
    source/scriptlib/List.c
    source/scriptlib/Parser.c
    source/scriptlib/ParserSet.c
    source/scriptlib/ScriptVariant.c
    source/scriptlib/Stack.c
    source/scriptlib/StackedSymbolTable.c
    source/scriptlib/SymbolTable.c
    source/scriptlib/ImportCache.c
)

set(SDL_PLATFORM_SOURCES
    sdl/control.c
    sdl/joysticks.c
    sdl/menu.c
    sdl/sblaster.c
    sdl/sdlport.c
    sdl/threads.c
    sdl/timer.c
    sdl/video.c
    sdl/videocommon.c
    sdl/SDL2_framerate.c
)

# Platform-specific sources
if(OPENBOR_PLATFORM STREQUAL "IOS")
    set(PLATFORM_SPECIFIC_SOURCES
        sdl/ios/ios_paths.m
        sdl/ios/touchcontrols.c
    )
    if(NOT APPLE)
        set_source_files_properties(sdl/ios/ios_paths.m PROPERTIES LANGUAGE C)
    endif()
elseif(OPENBOR_PLATFORM STREQUAL "WIN" OR OPENBOR_PLATFORM STREQUAL "LINUX" OR OPENBOR_PLATFORM STREQUAL "DARWIN")
    set(PLATFORM_SPECIFIC_SOURCES
        sdl/loadgl.c
        sdl/opengl.c
    )
endif()

# All OpenBOR engine sources
set(OPENBOR_ALL_SOURCES
    ${CORE_SOURCES}
    ${ADPCM_SOURCES}
    ${GAMELIB_SOURCES}
    ${GFXLIB_SOURCES}
    ${OPENBORSCRIPT_SOURCES}
    ${PNGLIB_SOURCES}
    ${PREPROCESSOR_SOURCES}
    ${RAMLIB_SOURCES}
    ${RANDLIB_SOURCES}
    ${SCRIPTLIB_SOURCES}
    ${SDL_PLATFORM_SOURCES}
    ${PLATFORM_SPECIFIC_SOURCES}
)

# -----------------------------------------------------------------------------
# Target Creation
# -----------------------------------------------------------------------------
if(OPENBOR_PLATFORM STREQUAL "IOS")
    add_executable(${TARGET_NAME} MACOSX_BUNDLE ${OPENBOR_ALL_SOURCES})
else()
    add_executable(${TARGET_NAME} ${OPENBOR_ALL_SOURCES})
endif()

# -----------------------------------------------------------------------------
# Include Directories
# -----------------------------------------------------------------------------
target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/sdl
    ${CMAKE_CURRENT_SOURCE_DIR}/source
    ${CMAKE_CURRENT_SOURCE_DIR}/source/adpcmlib
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gamelib
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gfxlib
    ${CMAKE_CURRENT_SOURCE_DIR}/source/openborscript
    ${CMAKE_CURRENT_SOURCE_DIR}/source/pnglib
    ${CMAKE_CURRENT_SOURCE_DIR}/source/preprocessorlib
    ${CMAKE_CURRENT_SOURCE_DIR}/source/ramlib
    ${CMAKE_CURRENT_SOURCE_DIR}/source/randlib
    ${CMAKE_CURRENT_SOURCE_DIR}/source/scriptlib
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    ${CMAKE_CURRENT_SOURCE_DIR}/android/jni/openbor/include
    ${CMAKE_CURRENT_SOURCE_DIR}/android/jni/openbor/include/sdl
    ${CMAKE_CURRENT_SOURCE_DIR}/android/jni/openbor/include/png
    ${CMAKE_CURRENT_SOURCE_DIR}/android/jni/openbor/include/ogg
    ${CMAKE_CURRENT_SOURCE_DIR}/android/jni/openbor/include/tremor
)

if(OPENBOR_PLATFORM STREQUAL "IOS")
    target_include_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/sdl/ios
    )
endif()

# -----------------------------------------------------------------------------
# Preprocessor Definitions & Compiler Options
# -----------------------------------------------------------------------------
target_compile_definitions(${TARGET_NAME} PRIVATE
    SDL=1
    SDL2=1
    VERSION_NAME="OpenBOR"
    VERSION_BUILD="6391"
)

if(OPENBOR_PLATFORM STREQUAL "IOS")
    target_compile_definitions(${TARGET_NAME} PRIVATE
        TARGET_OS_IPHONE=1
        __IPHONEOS__=1
        IOS=1
        DARWIN=1
        SKIP_CODE=1
        TREMOR=1
        OPENGLES=1
        NO_OPENGL=1
        OV_EXCLUDE_STATIC_CALLBACKS=1
    )

    set(OPENBOR_BUNDLE_IDENTIFIER "org.openbor.engine" CACHE STRING "iOS App Bundle Identifier")

    # Generate iOS Info.plist
    set(MACOSX_BUNDLE_EXECUTABLE_NAME "OpenBOR")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "${OPENBOR_BUNDLE_IDENTIFIER}")
    set(MACOSX_BUNDLE_BUNDLE_NAME "OpenBOR")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "3.0")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "6391")

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/ios/Info.plist.in"
        "${CMAKE_CURRENT_BINARY_DIR}/Info.plist"
        @ONLY
    )

    set_target_properties(${TARGET_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_BINARY_DIR}/Info.plist"
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${OPENBOR_BUNDLE_IDENTIFIER}"
        XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2" # 1 = iPhone, 2 = iPad
        XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "12.0"
        XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
        XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO"
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
        XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "YES"
    )

    # Link iOS system frameworks
    target_link_libraries(${TARGET_NAME} PRIVATE
        "-framework UIKit"
        "-framework Foundation"
        "-framework CoreGraphics"
        "-framework Metal"
        "-framework OpenGLES"
        "-framework AudioToolbox"
        "-framework AVFoundation"
        "-framework CoreAudio"
        "-framework GameController"
        "-framework QuartzCore"
        "-framework CoreMotion"
        z
        m
    )

elseif(OPENBOR_PLATFORM STREQUAL "WIN")
    target_compile_definitions(${TARGET_NAME} PRIVATE
        WIN=1
        OPENGL=1
        LOADGL=1
    )
    target_link_libraries(${TARGET_NAME} PRIVATE
        opengl32
        winmm
        imm32
        version
        ole32
        oleaut32
        uuid
        psapi
    )
elseif(OPENBOR_PLATFORM STREQUAL "DARWIN")
    target_compile_definitions(${TARGET_NAME} PRIVATE
        DARWIN=1
        OPENGL=1
        LOADGL=1
    )
    target_link_libraries(${TARGET_NAME} PRIVATE
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
        "-framework AudioUnit"
        "-framework CoreAudio"
        z
        m
    )
else() # Linux
    target_compile_definitions(${TARGET_NAME} PRIVATE
        LINUX=1
        OPENGL=1
        LOADGL=1
    )
    target_link_libraries(${TARGET_NAME} PRIVATE
        GL
        pthread
        dl
        m
        z
    )
endif()

# -----------------------------------------------------------------------------
# SDL2 & Media Dependencies Resolution
# -----------------------------------------------------------------------------
find_package(SDL2 QUIET)
if(TARGET SDL2::SDL2)
    target_link_libraries(${TARGET_NAME} PRIVATE SDL2::SDL2)
    message(STATUS "Found SDL2 target: SDL2::SDL2")
elseif(TARGET SDL2)
    target_link_libraries(${TARGET_NAME} PRIVATE SDL2)
    message(STATUS "Found SDL2 target: SDL2")
else()
    # Support SDL2 via direct library / framework search or user variable
    find_library(SDL2_LIBRARY NAMES SDL2 SDL2.xcframework)
    find_path(SDL2_INCLUDE_DIR NAMES SDL.h PATH_SUFFIXES SDL2)
    if(SDL2_LIBRARY AND SDL2_INCLUDE_DIR)
        target_link_libraries(${TARGET_NAME} PRIVATE ${SDL2_LIBRARY})
        target_include_directories(${TARGET_NAME} PRIVATE ${SDL2_INCLUDE_DIR})
        message(STATUS "Found SDL2 library: ${SDL2_LIBRARY}")
    else()
        message(STATUS "Note: SDL2 library will be linked from system/toolchain or fetched in CI.")
        target_link_libraries(${TARGET_NAME} PRIVATE SDL2)
    endif()
endif()

# PNG, Ogg, Vorbis / Tremor resolution
find_package(PNG QUIET)
if(PNG_FOUND)
    target_link_libraries(${TARGET_NAME} PRIVATE PNG::PNG)
else()
    target_link_libraries(${TARGET_NAME} PRIVATE png)
endif()

find_library(VORBISIDEC_LIB NAMES vorbisidec tremor)
if(VORBISIDEC_LIB)
    target_link_libraries(${TARGET_NAME} PRIVATE ${VORBISIDEC_LIB})
else()
    target_link_libraries(${TARGET_NAME} PRIVATE vorbisidec)
endif()

find_library(OGG_LIB NAMES ogg)
if(OGG_LIB)
    target_link_libraries(${TARGET_NAME} PRIVATE ${OGG_LIB})
else()
    target_link_libraries(${TARGET_NAME} PRIVATE ogg)
endif()

# Compiler warnings and optimizations
if(CMAKE_C_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
    target_compile_options(${TARGET_NAME} PRIVATE
        -Wall
        -Wno-unused-function
        -Wno-unused-variable
        -Wno-format-security
        -fsigned-char
    )
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${TARGET_NAME} PRIVATE -g -O0)
    else()
        target_compile_options(${TARGET_NAME} PRIVATE -O2)
    endif()
endif()
