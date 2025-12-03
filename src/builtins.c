#include "../include/shell.h"

/**
 * Check if command is a built-in
 */
int is_builtin(char *command) {
    const char *builtins[] = {
        "cd", "exit", "help", "history", "pwd", "echo", "export", "clear", NULL
    };

    for (int i = 0; builtins[i] != NULL; i++) {
        if (strcmp(command, builtins[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

/**
 * Execute built-in command
 */
int execute_builtin(Command *cmd) {
    if (strcmp(cmd->tokens[0], "cd") == 0) {
        return builtin_cd(cmd->tokens);
    } else if (strcmp(cmd->tokens[0], "exit") == 0) {
        return builtin_exit(cmd->tokens);
    } else if (strcmp(cmd->tokens[0], "help") == 0) {
        return builtin_help(cmd->tokens);
    } else if (strcmp(cmd->tokens[0], "history") == 0) {
        return builtin_history(cmd->tokens);
    } else if (strcmp(cmd->tokens[0], "pwd") == 0) {
        return builtin_pwd(cmd->tokens);
    } else if (strcmp(cmd->tokens[0], "echo") == 0) {
        return builtin_echo(cmd->tokens);
    } else if (strcmp(cmd->tokens[0], "export") == 0) {
        return builtin_export(cmd->tokens);
    } else if (strcmp(cmd->tokens[0], "clear") == 0) {
        return builtin_clear(cmd->tokens);
    }

    return -1;
}

/**
 * Change directory
 */
int builtin_cd(char **args) {
    char *path;

    if (args[1] == NULL) {
        /* No argument - go to home directory */
        path = getenv("HOME");
        if (!path) {
            print_error("HOME environment variable not set");
            return -1;
        }
    } else {
        path = args[1];
    }

    if (chdir(path) != 0) {
        print_error("Failed to change directory");
        return -1;
    }

    return 0;
}

/**
 * Exit shell
 */
int builtin_exit(char **args) {
    int exit_code = 0;

    if (args[1] != NULL) {
        exit_code = atoi(args[1]);
    }

    return exit_code;
}

/**
 * Print help information
 */
int builtin_help(char **args) {
    printf("%s", COLOR_CYAN);
    printf("\n==========================================================\n");
    printf("              Mini Shell - Built-in Commands              \n");
    printf("==========================================================\n");
    printf(" cd [dir]        - Change directory                       \n");
    printf(" pwd             - Print working directory                \n");
    printf(" echo [args]     - Print arguments                        \n");
    printf(" export VAR=val  - Set environment variable               \n");
    printf(" history         - Show command history                   \n");
    printf(" clear           - Clear the screen                       \n");
    printf(" help            - Show this help message                 \n");
    printf(" exit [code]     - Exit the shell                         \n");
    printf("----------------------------------------------------------\n");
    printf("                    Features Supported                    \n");
    printf("----------------------------------------------------------\n");
    printf(" Redirection:                                             \n");
    printf("   command > file    - Redirect output to file            \n");
    printf("   command >> file   - Append output to file              \n");
    printf("   command < file    - Redirect input from file           \n");
    printf("                                                           \n");
    printf(" Background:                                              \n");
    printf("   command &         - Run command in background          \n");
    printf("==========================================================\n");
    printf("%s\n", COLOR_RESET);

    return 0;
}

/**
 * Show command history
 */
int builtin_history(char **args) {
    print_history(g_history);
    return 0;
}

/**
 * Print working directory
 */
int builtin_pwd(char **args) {
    char cwd[MAX_PATH_SIZE];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
        return 0;
    } else {
        print_error("Failed to get current directory");
        return -1;
    }
}

/**
 * Echo command
 */
int builtin_echo(char **args) {
    for (int i = 1; args[i] != NULL; i++) {
        printf("%s", args[i]);
        if (args[i + 1] != NULL) {
            printf(" ");
        }
    }
    printf("\n");
    return 0;
}

/**
 * Export environment variable
 */
int builtin_export(char **args) {
    if (args[1] == NULL) {
        print_error("Usage: export VAR=value");
        return -1;
    }

    /* Parse VAR=value */
    char *equal_sign = strchr(args[1], '=');
    if (!equal_sign) {
        print_error("Invalid format. Use: export VAR=value");
        return -1;
    }

    /* Split variable name and value */
    *equal_sign = '\0';
    char *var_name = args[1];
    char *var_value = equal_sign + 1;

    #ifdef _WIN32
    if (_putenv_s(var_name, var_value) != 0) {
        print_error("Failed to set environment variable");
        return -1;
    }
    #else
    if (setenv(var_name, var_value, 1) != 0) {
        print_error("Failed to set environment variable");
        return -1;
    }
    #endif

    return 0;
}

/**
 * Clear screen
 */
int builtin_clear(char **args) {
    printf("\033[H\033[J");
    return 0;
}
