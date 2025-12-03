#include "../include/shell.h"

#ifdef _WIN32

/**
 * Find executable in PATH (Windows version)
 */
char* find_executable(const char *command) {
    static char full_path[MAX_PATH];
    char *path_env;
    char *path_copy;
    char *token;
    const char *extensions[] = {".exe", ".bat", ".cmd", ".com", NULL};

    /* Check if command already has extension or is absolute path */
    if (strchr(command, '\\') || strchr(command, '/') || strchr(command, '.')) {
        if (GetFileAttributesA(command) != INVALID_FILE_ATTRIBUTES) {
            strncpy(full_path, command, MAX_PATH - 1);
            full_path[MAX_PATH - 1] = '\0';
            return full_path;
        }
    }

    /* Get PATH environment variable */
    path_env = getenv("PATH");
    if (!path_env) {
        return NULL;
    }

    path_copy = strdup(path_env);
    if (!path_copy) {
        return NULL;
    }

    /* Try each directory in PATH */
    token = strtok(path_copy, ";");
    while (token != NULL) {
        /* Try each extension */
        for (int i = 0; extensions[i] != NULL; i++) {
            snprintf(full_path, MAX_PATH, "%s\\%s%s", token, command, extensions[i]);
            if (GetFileAttributesA(full_path) != INVALID_FILE_ATTRIBUTES) {
                free(path_copy);
                return full_path;
            }
        }

        /* Try without extension */
        snprintf(full_path, MAX_PATH, "%s\\%s", token, command);
        if (GetFileAttributesA(full_path) != INVALID_FILE_ATTRIBUTES) {
            free(path_copy);
            return full_path;
        }

        token = strtok(NULL, ";");
    }

    free(path_copy);
    return NULL;
}

/**
 * Execute a single command (Windows version)
 */
int execute_command(Command *cmd) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    char command_line[MAX_INPUT_SIZE * 2];
    DWORD exit_code;
    HANDLE h_input = INVALID_HANDLE_VALUE;
    HANDLE h_output = INVALID_HANDLE_VALUE;
    SECURITY_ATTRIBUTES sa;

    if (!cmd || cmd->token_count == 0) {
        return -1;
    }

    /* Build command line */
    command_line[0] = '\0';
    for (int i = 0; i < cmd->token_count; i++) {
        if (i > 0) strcat(command_line, " ");

        /* Quote arguments containing spaces */
        if (strchr(cmd->tokens[i], ' ')) {
            strcat(command_line, "\"");
            strcat(command_line, cmd->tokens[i]);
            strcat(command_line, "\"");
        } else {
            strcat(command_line, cmd->tokens[i]);
        }
    }

    /* Initialize structures */
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    /* Setup security attributes for handle inheritance */
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    /* Handle input redirection */
    if (cmd->input_file) {
        h_input = CreateFileA(cmd->input_file, GENERIC_READ, FILE_SHARE_READ,
                              &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (h_input == INVALID_HANDLE_VALUE) {
            print_error("Failed to open input file");
            return -1;
        }
        si.hStdInput = h_input;
        si.dwFlags |= STARTF_USESTDHANDLES;
    } else {
        si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    }

    /* Handle output redirection */
    if (cmd->output_file) {
        DWORD creation = cmd->append_output ? OPEN_ALWAYS : CREATE_ALWAYS;
        h_output = CreateFileA(cmd->output_file, GENERIC_WRITE, FILE_SHARE_READ,
                               &sa, creation, FILE_ATTRIBUTE_NORMAL, NULL);
        if (h_output == INVALID_HANDLE_VALUE) {
            print_error("Failed to open output file");
            if (h_input != INVALID_HANDLE_VALUE) CloseHandle(h_input);
            return -1;
        }

        /* If appending, seek to end */
        if (cmd->append_output) {
            SetFilePointer(h_output, 0, NULL, FILE_END);
        }

        si.hStdOutput = h_output;
        si.hStdError = h_output;
        si.dwFlags |= STARTF_USESTDHANDLES;
    } else {
        si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    }

    /* Try to find executable */
    char *exe_path = find_executable(cmd->tokens[0]);

    /* Create process */
    BOOL success = CreateProcessA(
        exe_path,           /* Application name */
        command_line,       /* Command line */
        NULL,               /* Process security attributes */
        NULL,               /* Thread security attributes */
        TRUE,               /* Inherit handles */
        0,                  /* Creation flags */
        NULL,               /* Environment */
        NULL,               /* Current directory */
        &si,                /* Startup info */
        &pi                 /* Process information */
    );

    /* Close file handles */
    if (h_input != INVALID_HANDLE_VALUE) CloseHandle(h_input);
    if (h_output != INVALID_HANDLE_VALUE) CloseHandle(h_output);

    if (!success) {
        fprintf(stderr, "%smini-shell: %s: command not found%s\n",
                COLOR_RED, cmd->tokens[0], COLOR_RESET);
        return -1;
    }

    if (cmd->background) {
        /* Background process */
        printf("[Process %lu running in background]\n", pi.dwProcessId);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return 0;
    } else {
        /* Wait for process to complete */
        WaitForSingleObject(pi.hProcess, INFINITE);
        GetExitCodeProcess(pi.hProcess, &exit_code);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return (int)exit_code;
    }
}

#else

/* POSIX version */
#include <fcntl.h>

/**
 * Execute a command with redirection support
 */
int execute_with_redirection(Command *cmd) {
    int input_fd = -1;
    int output_fd = -1;

    /* Handle input redirection */
    if (cmd->input_file) {
        input_fd = open(cmd->input_file, O_RDONLY);
        if (input_fd < 0) {
            print_error("Failed to open input file");
            return -1;
        }
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }

    /* Handle output redirection */
    if (cmd->output_file) {
        int flags = O_WRONLY | O_CREAT;
        if (cmd->append_output) {
            flags |= O_APPEND;
        } else {
            flags |= O_TRUNC;
        }

        output_fd = open(cmd->output_file, flags, 0644);
        if (output_fd < 0) {
            print_error("Failed to open output file");
            return -1;
        }
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }

    /* Execute the command */
    execvp(cmd->tokens[0], cmd->tokens);

    /* If execvp returns, there was an error */
    print_error("Failed to execute command");
    exit(EXIT_FAILURE);
}

/**
 * Execute a single command
 */
int execute_command(Command *cmd) {
    pid_t pid;
    int status;

    if (!cmd || cmd->token_count == 0) {
        return -1;
    }

    /* Fork a child process */
    pid = fork();

    if (pid < 0) {
        print_error("Failed to fork process");
        return -1;
    }

    if (pid == 0) {
        /* Child process */

        /* Reset signal handlers for child */
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);

        /* Execute with redirection if needed */
        if (cmd->input_file || cmd->output_file) {
            execute_with_redirection(cmd);
        } else {
            execvp(cmd->tokens[0], cmd->tokens);

            /* If execvp returns, there was an error */
            fprintf(stderr, "%smini-shell: %s: command not found%s\n",
                    COLOR_RED, cmd->tokens[0], COLOR_RESET);
            exit(EXIT_FAILURE);
        }
    } else {
        /* Parent process */
        if (cmd->background) {
            /* Background process */
            printf("[Process %d running in background]\n", pid);
            return 0;
        } else {
            /* Foreground process - wait for child */
            do {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));

            if (WIFEXITED(status)) {
                return WEXITSTATUS(status);
            } else if (WIFSIGNALED(status)) {
                return 128 + WTERMSIG(status);
            }
        }
    }

    return 0;
}

#endif

/**
 * Execute a command with redirection support (wrapper function for Windows)
 */
int execute_with_redirection(Command *cmd) {
    /* On Windows, redirection is handled in execute_command */
    #ifdef _WIN32
    return execute_command(cmd);
    #else
    /* This is implemented above for POSIX */
    return -1;
    #endif
}

/**
 * Execute piped commands (for future implementation)
 */
int execute_piped_commands(Command *cmd) {
    /* TODO: Implement pipe support */
    print_error("Pipe support not yet implemented");
    return -1;
}
