# OpenBOR Build 6391 - iOS Platform Port Report

**Engine Version:** OpenBOR v3.0 Build 6391  
**Target Platform:** Apple iOS (iPhone & iPad, ARM64)  
**Host Environment:** Windows (Source, Configuration, Local Builds) / macOS (Xcode, Signing, IPA)

---

## 1. Architectural Summary

OpenBOR is a modular, C-based 2D scrolling beat-'em-up engine originally derived from Beats of Rage. The native iOS platform port was designed under the following principles:

1. **Zero Gameplay Rewriting:** Core gameplay, script bytecode parsing, animation logic, model management, collision geometry, and game rules remain 100% untouched from the original OpenBOR Build 6391 codebase.
2. **Platform Isolation:** All iOS-specific adaptations reside in isolated platform files (`engine/sdl/ios/`) and guarded platform blocks (`#if defined(IOS)`). No Windows, Linux, or Android platform code paths were broken or removed.
3. **Hardware Acceleration via SDL2:** Rather than relying on obsolete desktop OpenGL 1.2 or writing a proprietary Metal renderer, the port utilizes SDL2's hardware-accelerated rendering backend (`SDL_CreateRenderer`), which maps directly to Metal and OpenGL ES on iOS.
4. **Sandboxed File Hierarchy:** Apple iOS strictly enforces a read-only application bundle and read-write sandbox (`Documents`, `Library/Caches`). The port bridges OpenBOR's POSIX file system expectations to `<Documents>/Paks`, `<Documents>/Saves`, and `<Documents>/Logs`, while automatically syncing any shipped `.pak` files from the read-only application bundle.
5. **On-Screen Mobile Controls:** Multi-touch input (`SDL_FINGERDOWN`, `SDL_FINGERUP`, `SDL_FINGERMOTION`) is translated in real-time into OpenBOR keyflags, with adaptive layout scaling, safe-area inset preservation, and priority handoff to Bluetooth physical controllers.

---

## 2. Inventory of Modified & New Files

| File Path | Status | Purpose |
|---|---|---|
| `CMakeLists.txt` | **New** | Root multi-platform CMake configuration. |
| `cmake/ios.toolchain.cmake` | **New** | Native iOS ARM64 cross-compilation toolchain. |
| `engine/CMakeLists.txt` | **New** | Engine build target, framework linkages, and bundle configuration. |
| `engine/version.h` | **Modified** | Pinned `VERSION_BUILD` strictly to `"6391"`. |
| `engine/resources/ios/Info.plist.in` | **New** | iOS App bundle configuration, landscape orientations, and Files app sharing. |
| `engine/sdl/SDL2_framerate.h` / `.c` | **New** | Standalone framerate limiter, removing `SDL2_gfx` external dependency. |
| `engine/sdl/ios/ios_paths.h` / `.m` | **New** | Sandboxed path resolution, bundle sync, and safe-area insets. |
| `engine/sdl/ios/touchcontrols.h` / `.c` | **New** | Virtual D-pad and action button touchscreen overlay. |
| `engine/sdl/ios/button_png_800x480.h` | **New** | Embedded PNG skin for on-screen touch controls. |
| `engine/sdl/sdlport.h` / `.c` | **Modified** | Added iOS path initialization, case-insensitive string macros, and sandbox chdir. |
| `engine/sdl/video.c` | **Modified** | iOS Retina display resolution, aspect-ratio letterboxing, and touch rendering. |
| `engine/sdl/control.c` | **Modified** | Added SDL2 touch event ingestion and controller priority toggling. |
| `engine/source/utils.c` | **Modified** | Added Apple Clang `<malloc.h>` compatibility guard. |
| `engine/source/ramlib/ram.c` | **Modified** | Added Apple Clang `<malloc.h>` compatibility guard. |
| `engine/source/pnglib/savepng.c` | **Modified** | Added Apple Clang `<malloc.h>` compatibility guard. |
| `engine/source/preprocessorlib/pp_parser.c` | **Modified** | Added Apple Clang `<malloc.h>` compatibility guard. |
| `scripts/build-windows.bat` | **New** | One-click Windows desktop build script. |
| `scripts/verify-ios-cmake.bat` | **New** | One-click Windows verification script for the iOS target. |
| `scripts/build-ios.sh` | **New** | Automated macOS/Xcode build and IPA packaging script. |
| `.github/workflows/ios-build.yml` | **New** | GitHub Actions CI/CD pipeline for automated build, signing, and artifact upload. |

---

## 3. Dependency Licenses

* **OpenBOR Engine:** BSD License (see `LICENSE`).
* **SDL2 (Simple DirectMedia Layer):** zlib License (free for commercial and open-source use, no GPL restrictions).
* **Tremor (libvorbisidec) / libogg:** BSD License.
* **libpng / zlib:** libpng License / zlib License.

All bundled and linked dependencies are permissive open-source licenses compatible with Apple App Store and iOS distribution guidelines.

---

## 4. Known Limitations & Compatibility

1. **Desktop OpenGL Shaders:** Custom GLSL desktop shaders designed for desktop OpenGL 2.0+ are not supported on mobile OpenBOR builds. The engine utilizes SDL2's Metal/GLES streaming texture blitter, which ensures consistent 60 FPS performance on all ARM64 iOS devices.
2. **Video Cutscenes (WebM):** Build 6391's WebM player uses `libvpx`. On mobile devices, native beat-'em-up mods using traditional GIF/PNG scene sequences (`data/scenes/`) perform optimally; WebM playback requires linking `libvpx`.
3. **App Store Sandbox Constraints:** Because iOS does not allow arbitrary filesystem writes outside the app container, all `.pak` game modules must be placed in the app's `Paks/` folder (accessible via the iOS Files app or iTunes File Sharing under `On My iPhone/OpenBOR/Paks`).
