#include "../include/shell.h"

/* Global variables */
History *g_history = NULL;
int g_last_exit_status = 0;
#ifdef _WIN32
volatile int g_interrupted = 0;
#else
volatile sig_atomic_t g_interrupted = 0;
#endif

int main(int argc, char **argv) {
    char input[MAX_INPUT_SIZE];
    Command *cmd = NULL;

    /* Initialize shell */
    printf("%s", COLOR_CYAN);
    printf("============================================\n");
    printf("    Mini Shell v1.0 - Welcome!            \n");
    printf("    Type 'help' for commands              \n");
    printf("    Type 'exit' to quit                   \n");
    printf("============================================\n");
    printf("%s\n", COLOR_RESET);

    /* Initialize history */
    g_history = init_history();
    if (!g_history) {
        print_error("Failed to initialize command history");
        return 1;
    }

    /* Setup signal handlers */
    setup_signal_handlers();

    /* Main shell loop */
    while (1) {
        /* Print prompt */
        print_prompt();

        /* Reset interrupt flag */
        g_interrupted = 0;

        /* Read input */
        if (!fgets(input, MAX_INPUT_SIZE, stdin)) {
            if (feof(stdin)) {
                printf("\n");
                break;
            }
            if (g_interrupted) {
                printf("\n");
                continue;
            }
            print_error("Error reading input");
            continue;
        }

        /* Remove newline */
        input[strcspn(input, "\n")] = 0;

        /* Skip empty lines */
        if (is_empty_line(input)) {
            continue;
        }

        /* Add to history */
        add_to_history(g_history, input);

        /* Parse command */
        cmd = parse_command(input);
        if (!cmd) {
            print_error("Failed to parse command");
            continue;
        }

        /* Skip if no tokens */
        if (cmd->token_count == 0) {
            free_command(cmd);
            continue;
        }

        /* Execute command */
        if (is_builtin(cmd->tokens[0])) {
            g_last_exit_status = execute_builtin(cmd);

            /* Handle exit command */
            if (strcmp(cmd->tokens[0], "exit") == 0) {
                free_command(cmd);
                break;
            }
        } else {
            g_last_exit_status = execute_command(cmd);
        }

        /* Free command */
        free_command(cmd);
    }

    /* Cleanup */
    free_history(g_history);

    printf("%s", COLOR_GREEN);
    printf("Goodbye! Thanks for using Mini Shell.\n");
    printf("%s", COLOR_RESET);

    return g_last_exit_status;
}
