# OpenBOR Build 6391 - Windows Development Guide

This guide describes how to develop, inspect, configure, and perform preliminary builds of the OpenBOR v3.0 Build 6391 iOS port from a Windows development machine.

---

## 1. Prerequisites

1. **Git for Windows** (v2.x or later)
2. **CMake** (v3.16 or later, 3.20+ recommended)
3. **C/C++ Compiler**:
   * Microsoft Visual Studio 2019 / 2022 / 2026 (Community or higher) with C++ Desktop Workload, **OR**
   * MinGW-w64 (GCC 8.x+) with Ninja.

---

## 2. Verifying the iOS CMake Configuration on Windows

You do not need a Mac to verify that the iOS target and CMake build scripts are syntactically and architecturally sound. Run:

```cmd
scripts\verify-ios-cmake.bat
```

Or execute via PowerShell:

```powershell
cmake -S . -B build-ios-verify -DOPENBOR_BUILD_IOS=ON -DCMAKE_BUILD_TYPE=Release
```

This will configure the target `OpenBOR`, generate the iOS `Info.plist` at `build-ios-verify/engine/Info.plist`, validate all include directories, and verify source-file linkage for ARM64 iOS without errors.

---

## 3. Building for Windows Desktop (CMake)

To test core engine behavior, model rendering, or PAK loading locally on Windows before packaging for iOS:

```cmd
scripts\build-windows.bat
```

Or manually:

```powershell
cmake -S . -B build-win -DCMAKE_BUILD_TYPE=Release
cmake --build build-win --config Release
```

The resulting executable will be located in:
```
build-win/engine/Release/OpenBOR.exe
```

---

## 4. Source Tree Organization

* **`engine/`**: Core OpenBOR v3.0 Build 6391 source files (`openbor.c`, `openborscript.c`, etc.).
* **`engine/sdl/`**: Platform abstraction layer using SDL2.
* **`engine/sdl/ios/`**: iOS-specific platform implementation:
  * `ios_paths.h` / `ios_paths.m`: Sandbox directory resolution and bundle synchronization.
  * `touchcontrols.h` / `touchcontrols.c`: Virtual touchscreen joystick and buttons overlay.
  * `button_png_800x480.h`: Embedded high-resolution button skin assets.
* **`cmake/`**: Platform toolchains (`ios.toolchain.cmake`).
* **`scripts/`**: Automation scripts for Windows and macOS.
* **`.github/workflows/`**: Continuous Integration and automated IPA packaging pipeline.
