@echo off
setlocal
echo ========================================================
echo   OpenBOR Build 6391 - Windows Desktop Build (CMake)
echo ========================================================

cd /d "%~dp0\.."

if not exist build-win mkdir build-win

cmake -S . -B build-win -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 (
    echo [ERROR] CMake configuration failed.
    exit /b 1
)

cmake --build build-win --config Release
if errorlevel 1 (
    echo [ERROR] Build failed.
    exit /b 1
)

echo.
echo ========================================================
echo   Build Successful!
echo   Output located in: build-win/engine/Release/
echo ========================================================
endlocal
