# Building Mini Shell on Windows

This guide explains how to build and run Mini Shell on Windows.

## Prerequisites

You need a C compiler for Windows. Choose one of the following:

### Option 1: MinGW (Recommended)
- Download MinGW from [MinGW.org](http://www.mingw.org/)
- Install and add to PATH
- Verify installation: `gcc --version`

### Option 2: MinGW-w64
- Download from [MinGW-w64](https://www.mingw-w64.org/)
- More modern and supports 64-bit

### Option 3: TDM-GCC
- Download from [TDM-GCC](https://jmeubank.github.io/tdm-gcc/)
- Easy installer for Windows

## Building the Project

### Method 1: Using the Build Script

Simply run the batch file:

```cmd
build.bat
```

This will:
1. Create `obj/` and `bin/` directories
2. Compile all source files
3. Link them into `bin\mini-shell.exe`

### Method 2: Manual Compilation

```cmd
REM Create directories
mkdir obj
mkdir bin

REM Compile source files
gcc -Wall -std=c11 -I./include -c src/main.c -o obj/main.o
gcc -Wall -std=c11 -I./include -c src/parser.c -o obj/parser.o
gcc -Wall -std=c11 -I./include -c src/executor.c -o obj/executor.o
gcc -Wall -std=c11 -I./include -c src/builtins.c -o obj/builtins.o
gcc -Wall -std=c11 -I./include -c src/history.c -o obj/history.o
gcc -Wall -std=c11 -I./include -c src/utils.c -o obj/utils.o

REM Link executable
gcc obj/main.o obj/parser.o obj/executor.o obj/builtins.o obj/history.o obj/utils.o -o bin/mini-shell.exe
```

## Running Mini Shell

After successful build:

```cmd
bin\mini-shell.exe
```

Or navigate to the bin directory:

```cmd
cd bin
mini-shell.exe
```

## Cleaning Build Files

To remove compiled files:

```cmd
clean.bat
```

Or manually:

```cmd
rmdir /s /q obj
rmdir /s /q bin
```

## Testing the Shell

Try these commands:

```
mini-shell$ help
mini-shell$ pwd
mini-shell$ echo Hello, Windows!
mini-shell$ dir
mini-shell$ cd ..
mini-shell$ history
mini-shell$ exit
```

## Windows-Specific Features

The shell automatically detects Windows and uses:
- **CreateProcess** instead of fork/exec
- **Windows file paths** (backslashes)
- **Windows environment variables** (USERPROFILE, etc.)
- **Windows API** for process management

## Supported Windows Commands

All standard Windows commands work:

- `dir` - List directory contents
- `cd` - Change directory
- `type` - Display file contents
- `copy` - Copy files
- `del` - Delete files
- `cls` - Clear screen (use `clear` as alias)
- And any other Windows executable!

## Redirection on Windows

Works just like Linux:

```
mini-shell$ dir > files.txt
mini-shell$ type files.txt
mini-shell$ echo "New line" >> files.txt
mini-shell$ findstr "New" < files.txt
```

## Troubleshooting

### "gcc not found"
- Make sure MinGW is installed and in your PATH
- Restart command prompt after installation

### Colors not showing properly
- Use Windows Terminal or ConEmu for better color support
- Or disable colors in the code if needed

### Permission denied
- Run as Administrator if needed
- Check antivirus settings

### Build errors
- Make sure you have the latest MinGW version
- Check that all source files are present
- Try cleaning and rebuilding: `clean.bat && build.bat`

## Differences from Linux Version

The Windows version has some differences:

1. **Process Creation**: Uses CreateProcess API instead of fork/exec
2. **Path Separator**: Uses backslash `\` instead of `/`
3. **Home Directory**: Uses USERPROFILE instead of HOME
4. **Signal Handling**: Simplified (no SIGCHLD support)
5. **Executables**: Looks for `.exe`, `.bat`, `.cmd` extensions

## Performance Notes

- First run may be slower due to Windows Defender scanning
- Add exclusion for the bin folder if needed
- Performance is generally good for typical shell operations

## Next Steps

After building successfully:

1. Try all built-in commands
2. Test with Windows system commands
3. Experiment with I/O redirection
4. Check command history
5. Customize as needed!

## Support

If you encounter issues:

1. Check this guide carefully
2. Verify GCC installation
3. Make sure all files are present
4. Check Windows version compatibility (Windows 7+)

---

**Happy Coding on Windows!**
