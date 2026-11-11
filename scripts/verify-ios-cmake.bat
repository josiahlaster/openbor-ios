@echo off
setlocal
echo ========================================================
echo   OpenBOR Build 6391 - Verify iOS CMake Target (Windows)
echo ========================================================

cd /d "%~dp0\.."

cmake -S . -B build-ios-verify -DOPENBOR_BUILD_IOS=ON -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 (
    echo.
    echo [ERROR] iOS CMake configuration failed.
    exit /b 1
)

echo.
echo ========================================================
echo   [SUCCESS] iOS CMake target configuration verified!
echo ========================================================
endlocal
