# OpenBOR Build 6391 - macOS & iOS Build Guide

This guide describes how to build, test on Simulator, and package native iOS applications (.ipa) for iPhone and iPad on a macOS workstation or CI runner.

---

## 1. Prerequisites

* **macOS**: macOS 13 (Ventura), macOS 14 (Sonoma), or later.
* **Xcode**: Xcode 14.x, 15.x, or 16.x with the iOS SDK installed.
* **Command Line Tools**: `xcode-select --install`
* **CMake**: `brew install cmake`

---

## 2. One-Click Native iOS Build & IPA Packaging

To build the ARM64 iOS application and generate an installable `.ipa` package in one step:

```bash
chmod +x scripts/build-ios.sh
./scripts/build-ios.sh Release
```

This script:
1. Configures the Xcode project using `cmake/ios.toolchain.cmake`.
2. Invokes `xcodebuild` targeting `generic/platform=iOS` for ARM64 architecture.
3. Assembles the `Payload/` folder with `OpenBOR.app`.
4. Packages and exports `build-ios/OpenBOR.ipa`.

---

## 3. Opening in Xcode (GUI)

To develop, profile with Instruments, or debug on a connected iPhone/iPad using the Xcode IDE:

1. Generate the Xcode project:
   ```bash
   cmake -S . -B build-ios \
     -G Xcode \
     -DCMAKE_TOOLCHAIN_FILE=cmake/ios.toolchain.cmake \
     -DCMAKE_SYSTEM_NAME=iOS \
     -DCMAKE_OSX_SYSROOT=iphoneos \
     -DCMAKE_OSX_ARCHITECTURES=arm64 \
     -DCMAKE_OSX_DEPLOYMENT_TARGET=12.0
   ```
2. Open `build-ios/OpenBOR.xcodeproj` in Xcode:
   ```bash
   open build-ios/OpenBOR.xcodeproj
   ```
3. In Xcode's project navigator:
   * Select the **OpenBOR** target.
   * Under **Signing & Capabilities**, select your **Team** (personal free Apple ID or paid Developer Account).
   * Connect your iPhone or iPad, select it as the run destination, and click **Run** (Cmd+R).

---

## 4. Building for iOS Simulator

To compile and launch inside the macOS iOS Simulator (x86_64 or arm64 Apple Silicon):

```bash
cmake -S . -B build-sim \
  -G Xcode \
  -DCMAKE_TOOLCHAIN_FILE=cmake/ios.toolchain.cmake \
  -DCMAKE_SYSTEM_NAME=iOS \
  -DCMAKE_OSX_SYSROOT=iphonesimulator \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=12.0

xcodebuild -project build-sim/OpenBOR.xcodeproj \
  -scheme OpenBOR \
  -configuration Debug \
  -destination "platform=iOS Simulator,name=iPhone 15" \
  build
```
