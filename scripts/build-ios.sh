#!/usr/bin/env bash
# ==============================================================================
# OpenBOR v3.0 Build 6391 - Native iOS Build & Packaging Script
# For macOS developer workstations and GitHub Actions macOS runners
# ==============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"

BUILD_DIR="${ROOT_DIR}/build-ios"
CONFIGURATION="${1:-Release}"
DEPLOYMENT_TARGET="12.0"
BUNDLE_ID="${BUNDLE_ID:-org.openbor.engine}"

echo "========================================================"
echo "  OpenBOR Build 6391: Native iOS (ARM64) Build Pipeline"
echo "  Configuration:     ${CONFIGURATION}"
echo "  Deployment Target: iOS ${DEPLOYMENT_TARGET}"
echo "  Bundle Identifier: ${BUNDLE_ID}"
echo "========================================================"

mkdir -p "${BUILD_DIR}"

echo ""
echo "--> Step 1: Generating Xcode project with CMake..."
cmake -S "${ROOT_DIR}" -B "${BUILD_DIR}" \
  -G Xcode \
  -DCMAKE_TOOLCHAIN_FILE="${ROOT_DIR}/cmake/ios.toolchain.cmake" \
  -DCMAKE_SYSTEM_NAME=iOS \
  -DCMAKE_OSX_SYSROOT=iphoneos \
  -DCMAKE_OSX_ARCHITECTURES=arm64 \
  -DCMAKE_OSX_DEPLOYMENT_TARGET="${DEPLOYMENT_TARGET}" \
  -DOPENBOR_BUNDLE_IDENTIFIER="${BUNDLE_ID}" \
  -DCMAKE_BUILD_TYPE="${CONFIGURATION}"

echo ""
echo "--> Step 2: Compiling ARM64 iOS App with xcodebuild..."
xcodebuild -project "${BUILD_DIR}/OpenBOR.xcodeproj" \
  -scheme OpenBOR \
  -configuration "${CONFIGURATION}" \
  -destination "generic/platform=iOS" \
  CODE_SIGN_IDENTITY="" \
  CODE_SIGNING_REQUIRED=NO \
  CODE_SIGNING_ALLOWED=NO \
  build

APP_PATH="${BUILD_DIR}/engine/${CONFIGURATION}-iphoneos/OpenBOR.app"

if [ ! -d "${APP_PATH}" ]; then
  # Fallback location for single-dir output
  APP_PATH=$(find "${BUILD_DIR}" -type d -name "OpenBOR.app" | head -n 1)
fi

if [ -z "${APP_PATH}" ] || [ ! -d "${APP_PATH}" ]; then
  echo "[ERROR] OpenBOR.app bundle not found in build output!"
  exit 1
fi

echo "--> Verified App Bundle: ${APP_PATH}"

echo ""
echo "--> Step 3: Packaging into IPA..."
PACKAGE_DIR="${BUILD_DIR}/Package"
rm -rf "${PACKAGE_DIR}"
mkdir -p "${PACKAGE_DIR}/Payload"

cp -R "${APP_PATH}" "${PACKAGE_DIR}/Payload/"

# Ensure bundled PAKs are present inside App bundle
if [ -d "${ROOT_DIR}/engine/resources/ios/Paks" ]; then
  mkdir -p "${PACKAGE_DIR}/Payload/OpenBOR.app/Paks"
  cp -f "${ROOT_DIR}/engine/resources/ios/Paks/"*.pak "${PACKAGE_DIR}/Payload/OpenBOR.app/Paks/" 2>/dev/null || true
fi

(cd "${PACKAGE_DIR}" && zip -qr -y "${BUILD_DIR}/OpenBOR.ipa" Payload)

echo ""
echo "========================================================"
echo "  [SUCCESS] iOS Build & Packaging Complete!"
echo "  Artifact: ${BUILD_DIR}/OpenBOR.ipa"
echo "========================================================"
