#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/* TODO: REMOVE THIS */
#include <assert.h>

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
char* get_token(FILE* file);
entry_t* get_entry(FILE* file);

entry_t* symbol_table_insert(symbol_table_t* symbol_table, entry_t entry);
entry_t* symbol_table_lookup(const symbol_table_t* symbol_table, char* name);
symbol_table_t* symbol_table_enter_scope(symbol_table_t* symbol_table);
symbol_table_t* symbol_table_exit_scope(const symbol_table_t* symbol_table);



int main(int argc, char *argv[])
{
    int is_eof = 0;
    entry_t *token = NULL;
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
    while (!feof(source)) {
        assert(0);
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
char* get_token(FILE* file)
{
    char buf[1];
    char temp_token[MAX_BUFFER_SIZE];
    char *token = NULL;
    int i = 0;

    /* return null if end of file */
    if (feof(file)) {
        return NULL;
    }

    while (i < MAX_BUFFER_SIZE || feof(file)) {

        /* read a character from the file */
        fscanf(file, "%c", &buf);
        if (buf[0] == 0) {
            return NULL;
        }


        /* if character if space, tab or newline */
        if (buf[0] == 32 || buf[0] == 9 || buf[0] == 10) {
            if (i == 0) {
                /* ignore if nothing is read */
                continue;
            } else {
                /* else send it */
                break;
                i++;
            }
        }
        /* if a special character is read */
        else if (buf[0] == '{'
                || buf[0] == '}'
                || buf[0] == '('
                || buf[0] == ')'
                || buf[0] == '['
                || buf[0] == ']'
                || buf[0] == '<'
                || buf[0] == '>'
                || buf[0] == '!'
                || buf[0] == '='
                || buf[0] == '|'
                || buf[0] == '.'
                || buf[0] == '*'
                || buf[0] == ';'
                || buf[0] == '-'
                || buf[0] == '/'
                || buf[0] == '+'
                || buf[0] == ',')
        {
            if (i == 0) {
                /* send it as a token if nothing is read */
                temp_token[0] = buf[0];
                i++;
            } else {
                /* rewind the file the send the token that was being read */
                fseek(file, -1, SEEK_CUR);
            }
            break;
        }

        temp_token[i] = buf[0];
        i++;
    }

    /* append the null character and send the token */
    temp_token[i] = '\0';
    token = (char *) malloc(strlen(temp_token) * sizeof(char));
    strcpy(token, temp_token);
    return token;
}

/**
 * Returns the next entry for the file descriptor and increment the file
 * position. Returns NULL if no more tokens or error.
 *
 * @param fd File descriptor
 * @return char* The next entry in symbol table
 */
entry_t* get_entry(FILE* file)
{
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
