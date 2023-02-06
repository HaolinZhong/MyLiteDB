//
// Created by haolin on 2/5/23.
//
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct InputContext {
    char *buffer;
    size_t buffer_size;
    ssize_t input_length;
} InputContext;

typedef enum MetaCommandResult {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED
} MetaCommandResult;

typedef enum PrepareStatementResult {
    PREPARE_STATEMENT_SUCCESS,
    PREPARE_STATEMENT_UNRECOGNIZED
} PrepareStatementResult;

typedef enum StatementType {
    INSERT_STATEMENT,
    SELECT_STATEMENT
} StatementType;

typedef struct Statement {
    StatementType type;
} Statement;

InputContext *new_input_context() {
    InputContext *input_context = (InputContext *) malloc(sizeof(InputContext));
    input_context->buffer = NULL;
    input_context->buffer_size = 0;
    input_context->input_length = 0;
    return input_context;
}

void print_prompt() {
    printf("MyLiteDB> ");
}

void read_input(InputContext *input_context) {

//    getline() reads an entire line from stream, storing the address
//    of the buffer containing the text into *lineptr.  The buffer is
//    null-terminated and includes the newline character, if one was
//    found.

    ssize_t byte_read = getline(&(input_context->buffer), &(input_context->buffer_size), stdin);

    if (byte_read <= 0) {
        printf("Error reading input.\n");
        exit(EXIT_FAILURE);
    }

    // getline return a string that ends with \n\0. we want to ignore the \n.
    input_context->input_length = byte_read - 1;
    input_context->buffer[byte_read - 1] = '\0';
}

void clean_input_context(InputContext *input_context) {
    free(input_context->buffer);
    free(input_context);
}

MetaCommandResult execute_meta_command(InputContext *input_context) {
    if (strcmp(input_context->buffer, ".exit") == 0) {
        clean_input_context(input_context);
        printf("Goodbye!\n");
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED;
    }
}

PrepareStatementResult  prepare_statement(InputContext* input_context, Statement* statement) {
    if (strncmp(input_context->buffer, "insert", 6) == 0) {
        statement->type = INSERT_STATEMENT;
        return PREPARE_STATEMENT_SUCCESS;
    }

    if (strncmp(input_context->buffer, "select", 6) == 0) {
        statement->type = SELECT_STATEMENT;
        return PREPARE_STATEMENT_SUCCESS;
    }

    return PREPARE_STATEMENT_UNRECOGNIZED;
}

void execute_statement(Statement* statement) {
    switch (statement->type) {
        case INSERT_STATEMENT:
            printf("Here we will do a insert\n");
            break;
        case SELECT_STATEMENT:
            printf("Here we will do a select\n");
            break;
    }
}

int main(int argc, char *argv[]) {
    InputContext *input_context = new_input_context();
    while (1) {
        print_prompt();
        read_input(input_context);

        if (input_context->buffer[0] == '.') {
            switch (execute_meta_command(input_context)) {
                case META_COMMAND_SUCCESS:
                    continue;
                case META_COMMAND_UNRECOGNIZED:
                    printf("Unrecognized command '%s'.\n", input_context->buffer);
                    continue;
            }
        }

        Statement statement;
        switch (prepare_statement(input_context, &statement)) {
            case PREPARE_STATEMENT_SUCCESS:
                break;
            case PREPARE_STATEMENT_UNRECOGNIZED:
                printf("Unrecognized statement '%s'.\n", input_context->buffer);
                continue;
        }

        execute_statement(&statement);
        printf("Statement executed.\n");
    }
}

