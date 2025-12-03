#include "../include/shell.h"

/**
 * Tokenize input string into array of tokens
 */
int tokenize(char *input, char **tokens) {
    int count = 0;
    char *token;

    #ifdef _WIN32
    /* Windows: use strtok */
    token = strtok(input, " \t\n");
    while (token != NULL && count < MAX_NUM_TOKENS) {
        tokens[count] = strdup(token);
        if (!tokens[count]) {
            /* Free previously allocated tokens on error */
            for (int i = 0; i < count; i++) {
                free(tokens[i]);
            }
            return -1;
        }
        count++;
        token = strtok(NULL, " \t\n");
    }
    #else
    /* POSIX: use strtok_r (thread-safe) */
    char *saveptr;
    token = strtok_r(input, " \t\n", &saveptr);
    while (token != NULL && count < MAX_NUM_TOKENS) {
        tokens[count] = strdup(token);
        if (!tokens[count]) {
            /* Free previously allocated tokens on error */
            for (int i = 0; i < count; i++) {
                free(tokens[i]);
            }
            return -1;
        }
        count++;
        token = strtok_r(NULL, " \t\n", &saveptr);
    }
    #endif

    return count;
}

/**
 * Parse command string and create Command structure
 */
Command* parse_command(char *input) {
    Command *cmd = (Command*)malloc(sizeof(Command));
    if (!cmd) {
        return NULL;
    }

    /* Initialize command structure */
    memset(cmd, 0, sizeof(Command));
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->append_output = 0;
    cmd->background = 0;
    cmd->pipe_count = 0;

    /* Make a copy of input for processing */
    char *input_copy = strdup(input);
    if (!input_copy) {
        free(cmd);
        return NULL;
    }

    /* Check for background process */
    int len = strlen(input_copy);
    if (len > 0 && input_copy[len - 1] == '&') {
        cmd->background = 1;
        input_copy[len - 1] = '\0';
    }

    /* Tokenize input */
    char *tokens[MAX_NUM_TOKENS];
    int token_count = tokenize(input_copy, tokens);

    free(input_copy);

    if (token_count < 0) {
        free(cmd);
        return NULL;
    }

    /* Parse tokens for redirection and pipes */
    int cmd_token_idx = 0;
    for (int i = 0; i < token_count; i++) {
        if (strcmp(tokens[i], ">") == 0) {
            /* Output redirection */
            if (i + 1 < token_count) {
                cmd->output_file = strdup(tokens[i + 1]);
                cmd->append_output = 0;
                free(tokens[i]);
                free(tokens[i + 1]);
                i++;
            }
        } else if (strcmp(tokens[i], ">>") == 0) {
            /* Append output redirection */
            if (i + 1 < token_count) {
                cmd->output_file = strdup(tokens[i + 1]);
                cmd->append_output = 1;
                free(tokens[i]);
                free(tokens[i + 1]);
                i++;
            }
        } else if (strcmp(tokens[i], "<") == 0) {
            /* Input redirection */
            if (i + 1 < token_count) {
                cmd->input_file = strdup(tokens[i + 1]);
                free(tokens[i]);
                free(tokens[i + 1]);
                i++;
            }
        } else if (strcmp(tokens[i], "|") == 0) {
            /* Pipe - for future implementation */
            cmd->pipe_count++;
            free(tokens[i]);
        } else {
            /* Regular command token */
            if (cmd_token_idx < MAX_NUM_TOKENS) {
                cmd->tokens[cmd_token_idx] = tokens[i];
                cmd_token_idx++;
            }
        }
    }

    cmd->token_count = cmd_token_idx;
    cmd->tokens[cmd_token_idx] = NULL;

    return cmd;
}

/**
 * Free command structure
 */
void free_command(Command *cmd) {
    if (!cmd) return;

    /* Free tokens */
    for (int i = 0; i < cmd->token_count; i++) {
        if (cmd->tokens[i]) {
            free(cmd->tokens[i]);
        }
    }

    /* Free file names */
    if (cmd->input_file) {
        free(cmd->input_file);
    }
    if (cmd->output_file) {
        free(cmd->output_file);
    }

    free(cmd);
}
