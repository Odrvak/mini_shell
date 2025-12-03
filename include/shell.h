#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

/* Windows-specific includes */
#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>
    #include <io.h>
    #include <process.h>
    #define getcwd _getcwd
    #define chdir _chdir
    #define strdup _strdup
    #define popen _popen
    #define pclose _pclose
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
#endif

/* Constants */
#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
#define MAX_HISTORY_SIZE 100
#define MAX_PATH_SIZE 256

/* Color codes for better UI */
#ifdef _WIN32
/* Windows CMD doesn't support ANSI colors by default - disable them */
#define COLOR_RESET   ""
#define COLOR_RED     ""
#define COLOR_GREEN   ""
#define COLOR_YELLOW  ""
#define COLOR_BLUE    ""
#define COLOR_MAGENTA ""
#define COLOR_CYAN    ""
#else
/* ANSI colors for Unix/Linux terminals */
#define COLOR_RESET   "\x1b[0m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#endif

/* Command structure */
typedef struct {
    char *tokens[MAX_NUM_TOKENS];
    int token_count;
    char *input_file;
    char *output_file;
    int append_output;
    int background;
    int pipe_count;
} Command;

/* History structure */
typedef struct {
    char **commands;
    int count;
    int capacity;
} History;

/* Parser functions - parser.c */
Command* parse_command(char *input);
void free_command(Command *cmd);
int tokenize(char *input, char **tokens);

/* Executor functions - executor.c */
int execute_command(Command *cmd);
int execute_piped_commands(Command *cmd);
int execute_with_redirection(Command *cmd);
#ifdef _WIN32
char* find_executable(const char *command);
#endif

/* Built-in commands - builtins.c */
int is_builtin(char *command);
int execute_builtin(Command *cmd);
int builtin_cd(char **args);
int builtin_exit(char **args);
int builtin_help(char **args);
int builtin_history(char **args);
int builtin_pwd(char **args);
int builtin_echo(char **args);
int builtin_export(char **args);
int builtin_clear(char **args);

/* History functions - history.c */
History* init_history(void);
void add_to_history(History *hist, char *command);
void print_history(History *hist);
void free_history(History *hist);
char* get_history_command(History *hist, int index);

/* Utility functions - utils.c */
void print_prompt(void);
void print_error(char *message);
void print_success(char *message);
char* trim_whitespace(char *str);
int is_empty_line(char *line);
void setup_signal_handlers(void);
void handle_sigint(int sig);
#ifndef _WIN32
void handle_sigchld(int sig);
#endif

/* Global variables */
extern History *g_history;
extern int g_last_exit_status;
#ifdef _WIN32
extern volatile int g_interrupted;
#else
extern volatile sig_atomic_t g_interrupted;
#endif

#endif /* SHELL_H */
