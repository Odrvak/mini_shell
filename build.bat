@echo off
REM Build script for Mini Shell on Windows

echo ========================================
echo   Building Mini Shell for Windows
echo ========================================
echo.

REM Create directories
if not exist "bin" mkdir bin
if not exist "obj" mkdir obj

REM Compiler settings
set CC=gcc
set CFLAGS=-Wall -Wextra -std=c11 -I./include
set LDFLAGS=
set SRC_DIR=src
set OBJ_DIR=obj
set BIN_DIR=bin

echo Compiling source files...

REM Compile each source file
%CC% %CFLAGS% -c %SRC_DIR%\main.c -o %OBJ_DIR%\main.o
if %errorlevel% neq 0 goto :error

%CC% %CFLAGS% -c %SRC_DIR%\parser.c -o %OBJ_DIR%\parser.o
if %errorlevel% neq 0 goto :error

%CC% %CFLAGS% -c %SRC_DIR%\executor.c -o %OBJ_DIR%\executor.o
if %errorlevel% neq 0 goto :error

%CC% %CFLAGS% -c %SRC_DIR%\builtins.c -o %OBJ_DIR%\builtins.o
if %errorlevel% neq 0 goto :error

%CC% %CFLAGS% -c %SRC_DIR%\history.c -o %OBJ_DIR%\history.o
if %errorlevel% neq 0 goto :error

%CC% %CFLAGS% -c %SRC_DIR%\utils.c -o %OBJ_DIR%\utils.o
if %errorlevel% neq 0 goto :error

echo.
echo Linking executable...
%CC% %OBJ_DIR%\main.o %OBJ_DIR%\parser.o %OBJ_DIR%\executor.o %OBJ_DIR%\builtins.o %OBJ_DIR%\history.o %OBJ_DIR%\utils.o %LDFLAGS% -o %BIN_DIR%\mini-shell.exe
if %errorlevel% neq 0 goto :error

echo.
echo ========================================
echo   Build successful!
echo   Run with: bin\mini-shell.exe
echo ========================================
goto :end

:error
echo.
echo ========================================
echo   Build failed!
echo ========================================
exit /b 1

:end
