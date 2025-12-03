# Mini Shell

A lightweight, feature-rich Unix shell implementation written in C. This project demonstrates core operating system concepts including process management, I/O redirection, signal handling, and command parsing.

## Features

### Core Functionality
- **Command Execution**: Execute any system command available in your PATH
- **Built-in Commands**: Essential shell commands implemented natively
- **Command History**: Keep track of previously executed commands
- **I/O Redirection**: Support for input/output redirection operators
- **Background Processes**: Run commands in the background
- **Signal Handling**: Proper handling of Ctrl+C, Ctrl+Z, and other signals
- **Colorful UI**: Enhanced user experience with colored output

### Built-in Commands

| Command | Description | Usage |
|---------|-------------|-------|
| `cd` | Change directory | `cd [directory]` |
| `pwd` | Print working directory | `pwd` |
| `echo` | Display a line of text | `echo [args...]` |
| `export` | Set environment variable | `export VAR=value` |
| `history` | Show command history | `history` |
| `clear` | Clear the screen | `clear` |
| `help` | Display help information | `help` |
| `exit` | Exit the shell | `exit [code]` |

### Redirection Operators

```bash
# Output redirection
command > file.txt          # Write output to file (overwrite)
command >> file.txt         # Append output to file

# Input redirection
command < input.txt         # Read input from file

# Background execution
command &                   # Run command in background
```

## Project Structure

```
mini-shell/
├── src/
│   ├── main.c          # Main shell loop and initialization
│   ├── parser.c        # Command parsing and tokenization
│   ├── executor.c      # Command execution and process management
│   ├── builtins.c      # Built-in command implementations
│   ├── history.c       # Command history management
│   └── utils.c         # Utility functions and signal handlers
├── include/
│   └── shell.h         # Header file with structures and prototypes
├── tests/              # Test files (for future development)
├── docs/               # Documentation (for future development)
├── examples/           # Example scripts
├── Makefile            # Build configuration
└── README.md           # This file
```

## Building the Project

### Prerequisites
- GCC compiler (or any C11 compatible compiler)
- Make build system
- POSIX-compliant operating system (Linux, macOS, WSL)

### Compilation

```bash
# Build the project
make

# Build with debug symbols
make debug

# Build optimized release version
make release

# Clean build files
make clean

# Rebuild from scratch
make rebuild
```

### Running

```bash
# Build and run
make run

# Or run directly
./bin/mini-shell
```

## Usage Examples

### Basic Commands
```bash
# Change directory
mini-shell$ cd /home/user/documents

# Print current directory
mini-shell$ pwd
/home/user/documents

# List files
mini-shell$ ls -la

# Echo text
mini-shell$ echo Hello, World!
Hello, World!
```

### Redirection
```bash
# Redirect output to file
mini-shell$ ls -la > files.txt

# Append to file
mini-shell$ echo "New line" >> files.txt

# Read from file
mini-shell$ wc -l < files.txt
```

### Background Processes
```bash
# Run long-running command in background
mini-shell$ sleep 60 &
[Process 12345 running in background]
```

### Environment Variables
```bash
# Set environment variable
mini-shell$ export MY_VAR=hello

# Use environment variable
mini-shell$ echo $MY_VAR
hello
```

### History
```bash
# View command history
mini-shell$ history
  1  ls -la
  2  cd /home/user
  3  pwd
  4  echo Hello
```

## Development

### Code Style
- Follow standard C coding conventions
- Use meaningful variable and function names
- Add comments for complex logic
- Keep functions focused and modular

### Memory Management
- All allocated memory is properly freed
- No memory leaks (verified with valgrind)
- Proper error handling for allocation failures

### Adding New Features

To add a new built-in command:

1. Add function prototype in `include/shell.h`
2. Implement the function in `src/builtins.c`
3. Add command name to `is_builtin()` function
4. Add case in `execute_builtin()` function
5. Update help text in `builtin_help()`

## Testing

```bash
# Run with valgrind for memory leak detection
make valgrind

# Run static analysis
make check

# Format code
make format
```

## Installation

```bash
# Install to /usr/local/bin (requires sudo)
make install

# Now you can run from anywhere
mini-shell

# Uninstall
make uninstall
```

## Technical Details

### Process Management
- Uses `fork()` to create child processes
- `execvp()` for command execution
- `waitpid()` for process synchronization
- Proper handling of zombie processes

### Signal Handling
- **SIGINT (Ctrl+C)**: Interrupt current foreground process
- **SIGCHLD**: Reap zombie processes automatically
- **SIGTSTP (Ctrl+Z)**: Ignored (can be implemented for job control)
- **SIGQUIT (Ctrl+\\)**: Ignored

### Input/Output Redirection
- Uses file descriptors and `dup2()` for redirection
- Supports both input (`<`) and output (`>`, `>>`) redirection
- Proper error handling for file operations

### Command Parsing
- Tokenization using `strtok_r()`
- Support for multiple arguments
- Handles special characters (`>`, `<`, `&`, `|`)
- Whitespace trimming and empty line detection

## Future Enhancements

Planned features for future versions:

- [ ] Pipe support (`command1 | command2`)
- [ ] Job control (fg, bg, jobs commands)
- [ ] Command-line editing with arrow keys
- [ ] Tab completion
- [ ] Command aliases
- [ ] Shell scripting support
- [ ] Globbing (wildcards: *, ?)
- [ ] Conditional execution (`&&`, `||`)
- [ ] Command substitution
- [ ] Configuration file (~/.minishellrc)

## Learning Objectives

This project helps understand:

1. **Operating System Concepts**
   - Process creation and management
   - Inter-process communication
   - Signal handling
   - File descriptors and I/O

2. **C Programming**
   - Memory management
   - String manipulation
   - Structures and pointers
   - Modular code organization

3. **System Programming**
   - POSIX system calls
   - Error handling
   - Signal safety
   - Resource management

## Common Issues and Solutions

### Issue: Permission Denied
```bash
# Make sure the binary is executable
chmod +x bin/mini-shell
```

### Issue: Command Not Found
```bash
# Verify the command exists in your PATH
which <command>

# Or use absolute path
/bin/ls
```

### Issue: Segmentation Fault
```bash
# Run with valgrind to debug
make valgrind
```

## Contributing

Contributions are welcome! Please follow these guidelines:

1. Fork the repository
2. Create a feature branch
3. Write clear commit messages
4. Test your changes thoroughly
5. Submit a pull request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Author

Created as an educational project to demonstrate C programming and operating system concepts.

## Acknowledgments

- Inspired by traditional Unix shells (bash, sh, zsh)
- Built with knowledge from operating systems textbooks and courses
- Thanks to the open-source community for resources and inspiration

## Resources

- [Advanced Programming in the UNIX Environment](https://www.apuebook.com/)
- [The Linux Programming Interface](https://man7.org/tlpi/)
- [GNU C Library Documentation](https://www.gnu.org/software/libc/manual/)
- [POSIX Standards](https://pubs.opengroup.org/onlinepubs/9699919799/)

---

**Happy Coding!** 🚀
