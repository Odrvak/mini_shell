@echo off
REM Clean build files for Mini Shell on Windows

echo Cleaning build files...

if exist "bin" (
    rmdir /s /q bin
    echo Removed bin directory
)

if exist "obj" (
    rmdir /s /q obj
    echo Removed obj directory
)

echo.
echo Clean complete!
