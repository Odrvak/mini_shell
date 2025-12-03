#include "../include/shell.h"

/**
 * Initialize command history
 */
History* init_history(void) {
    History *hist = (History*)malloc(sizeof(History));
    if (!hist) {
        return NULL;
    }

    hist->capacity = MAX_HISTORY_SIZE;
    hist->count = 0;
    hist->commands = (char**)malloc(sizeof(char*) * hist->capacity);

    if (!hist->commands) {
        free(hist);
        return NULL;
    }

    for (int i = 0; i < hist->capacity; i++) {
        hist->commands[i] = NULL;
    }

    return hist;
}

/**
 * Add command to history
 */
void add_to_history(History *hist, char *command) {
    if (!hist || !command) {
        return;
    }

    /* Don't add empty commands or duplicates of last command */
    if (strlen(command) == 0) {
        return;
    }

    if (hist->count > 0 && strcmp(hist->commands[hist->count - 1], command) == 0) {
        return;
    }

    /* If history is full, remove oldest command */
    if (hist->count >= hist->capacity) {
        free(hist->commands[0]);

        /* Shift all commands down */
        for (int i = 0; i < hist->capacity - 1; i++) {
            hist->commands[i] = hist->commands[i + 1];
        }
        hist->count--;
    }

    /* Add new command */
    hist->commands[hist->count] = strdup(command);
    if (hist->commands[hist->count]) {
        hist->count++;
    }
}

/**
 * Print command history
 */
void print_history(History *hist) {
    if (!hist) {
        return;
    }

    if (hist->count == 0) {
        printf("%sNo commands in history%s\n", COLOR_YELLOW, COLOR_RESET);
        return;
    }

    printf("%s", COLOR_CYAN);
    printf("\n===========================================================\n");
    printf("                    Command History                        \n");
    printf("===========================================================\n");
    printf("%s", COLOR_RESET);

    for (int i = 0; i < hist->count; i++) {
        printf("%s%4d%s  %s\n", COLOR_GREEN, i + 1, COLOR_RESET, hist->commands[i]);
    }
    printf("\n");
}

/**
 * Get command from history by index
 */
char* get_history_command(History *hist, int index) {
    if (!hist || index < 0 || index >= hist->count) {
        return NULL;
    }

    return hist->commands[index];
}

/**
 * Free command history
 */
void free_history(History *hist) {
    if (!hist) {
        return;
    }

    /* Free all command strings */
    for (int i = 0; i < hist->count; i++) {
        if (hist->commands[i]) {
            free(hist->commands[i]);
        }
    }

    /* Free commands array */
    free(hist->commands);

    /* Free history structure */
    free(hist);
}
