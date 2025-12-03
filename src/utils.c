#include "../include/shell.h"
#include <ctype.h>

/**
 * Print command prompt
 */
void print_prompt(void) {
    char cwd[MAX_PATH_SIZE];
    char *display_path = cwd;

    #ifdef _WIN32
    char *home = getenv("USERPROFILE");
    #else
    char *home = getenv("HOME");
    #endif

    /* Get current directory */
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        /* Replace home directory with ~ */
        if (home && strncmp(cwd, home, strlen(home)) == 0) {
            static char short_path[MAX_PATH_SIZE];
            snprintf(short_path, sizeof(short_path), "~%s", cwd + strlen(home));
            display_path = short_path;
        }
    } else {
        display_path = "???";
    }

    /* Print colorful prompt */
    printf("%s[%smini-shell%s] %s%s%s\n",
           COLOR_CYAN, COLOR_MAGENTA, COLOR_CYAN,
           COLOR_BLUE, display_path, COLOR_RESET);
    printf("%s$ %s", COLOR_GREEN, COLOR_RESET);
    fflush(stdout);
}

/**
 * Print error message
 */
void print_error(char *message) {
    fprintf(stderr, "%s[ERROR]%s %s\n", COLOR_RED, COLOR_RESET, message);
}

/**
 * Print success message
 */
void print_success(char *message) {
    printf("%s[SUCCESS]%s %s\n", COLOR_GREEN, COLOR_RESET, message);
}

/**
 * Trim whitespace from string
 */
char* trim_whitespace(char *str) {
    char *end;

    /* Trim leading space */
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0) {
        return str;
    }

    /* Trim trailing space */
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    /* Write new null terminator */
    end[1] = '\0';

    return str;
}

/**
 * Check if line is empty
 */
int is_empty_line(char *line) {
    if (!line) return 1;

    char *trimmed = trim_whitespace(line);
    return (strlen(trimmed) == 0);
}

/**
 * Signal handler for SIGINT (Ctrl+C)
 */
void handle_sigint(int sig) {
    g_interrupted = 1;
    printf("\n");
    print_prompt();
    fflush(stdout);
}

#ifndef _WIN32
/**
 * Signal handler for SIGCHLD (child process termination) - POSIX only
 */
void handle_sigchld(int sig) {
    /* Reap zombie processes */
    int saved_errno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0) {
        continue;
    }
    errno = saved_errno;
}
#endif

/**
 * Setup signal handlers
 */
void setup_signal_handlers(void) {
    #ifdef _WIN32
    /* Windows: Use simple signal() function */
    signal(SIGINT, handle_sigint);
    #else
    /* POSIX: Use sigaction for better control */
    struct sigaction sa_int;
    struct sigaction sa_chld;

    /* Handle SIGINT (Ctrl+C) */
    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_int, NULL);

    /* Handle SIGCHLD (child process termination) */
    sa_chld.sa_handler = handle_sigchld;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa_chld, NULL);

    /* Ignore SIGTSTP (Ctrl+Z) for now */
    signal(SIGTSTP, SIG_IGN);

    /* Ignore SIGQUIT (Ctrl+\) */
    signal(SIGQUIT, SIG_IGN);
    #endif
}
