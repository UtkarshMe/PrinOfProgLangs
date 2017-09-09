#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/* the maximum token size */
#define MAX_TOKEN_SIZE 255
#define MAX_BUFFER_SIZE 500

/* the data type of identifier */
typedef enum kind_enum {
    VOID,
    INT,
    FLOAT,
    DOUBLE
} kind_t;

/* the type of identifier */
typedef enum type_enum {
    PARAMETER,
    VARIABLE,
    FUNCTION,
    CLASS
} type_t;

/* the symbol table entry */
typedef struct entry_struct {
    char *identifier;
    type_t type;
    kind_t kind;
} entry_t;

/* the symbol table structure */
typedef struct symbol_table_struct {
    char *identifier;
    type_t type;
    kind_t kind;
} symbol_table_t;



/* function declarations */
char* get_token(const FILE *file);
void delete_token(char* token);

entry_t* symbol_table_insert(symbol_table_t* symbol_table, entry_t entry);
entry_t* symbol_table_lookup(const symbol_table_t* symbol_table, char* name);
symbol_table_t* symbol_table_enter_scope(symbol_table_t* symbol_table);
symbol_table_t* symbol_table_exit_scope(const symbol_table_t* symbol_table);



int main(int argc, char *argv[])
{
    int is_eof = 0;
    char *token = NULL;
    FILE *source = NULL;

    /* Check if a file was passed as an argument */
    if (argc <= 1) {
        fprintf(stderr, "The program expects a file as an argument\n");
        exit(1);
    }

    /* Check if file was readable */
    source = fopen(argv[1], "r");
    if (source == NULL) {
        fprintf(stderr, "Could not open file\n");
        exit(1);
    }


    /* Loop through the file till eof */
    while (!is_eof) {

        /* Get the next token */
        token = get_token(source);
        if (token == NULL) {
            is_eof = 1;
            break;
        }

        printf("%s\n", token);
    }


    /* Close the file */
    fclose(source);

    return 0;
}


/**
 * Returns the next token for the file descriptor and increment the file
 * position. Returns NULL if no more tokens or error.
 *
 * @param fd File descriptor
 * @return char* The next token
 */
char* get_token(const FILE *file)
{
    return NULL;
}

/**
 * Delete the token read and free the memory.
 *
 * @param char* Token to be deleted
 */
void delete_token(char* token)
{
    free(token);
}

/**
 * Insert entry if not already in the current scope. Error if a symbole of the
 * same name is already present in the current scope
 *
 * @param symbol_table_t The symbol table to search in
 * @param entry_t The entry to add to the table
 * @return entry_t Pointer to the new entry. NULL if error
 */
entry_t* symbol_table_insert(symbol_table_t* symbol_table, entry_t entry)
{
    return NULL;
}

/**
 * Search the symbol table for the given indentifier name and return a pointer
 * to the latest entry.
 *
 * @param symbol_table_t The pointer to the symbol table to search in
 * @param char* The name of the identifier
 * @return entry_t* The pointer to the symbol table entry. NULL if not found.
 */
entry_t* symbol_table_lookup(const symbol_table_t* symbol_table, char* name)
{
    return NULL;
}

/**
 * Create a new entry in the given symbol table and return a pointer to it
 *
 * @param symbol_table_t* The pointer to the symbol table to create entry into
 * @return symbol_table_t* The pointer to the new symbol table. NULL if error
 */
symbol_table_t* symbol_table_enter_scope(symbol_table_t* symbol_table)
{
    return NULL;
}

/**
 * Return the parent symbol table by exiting the scope of the current one
 *
 * @param symbol_table_t* The pointer to the current symbol table
 * @return symbol_table_t* The pointer to the parent symbol table. NULL if not
 *      found
 */
symbol_table_t* symbol_table_exit_scope(const symbol_table_t* symbol_table)
{
    return NULL;
}
