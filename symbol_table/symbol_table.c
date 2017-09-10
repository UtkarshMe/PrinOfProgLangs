#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


/* the maximum token size */
#define MAX_TOKEN_SIZE 255
#define MAX_BUFFER_SIZE 500


int is_param = 0;
int is_func = 0;
int scope_level = 0;


/* the data type of identifier */
typedef enum kind_enum {
    PARAMETER,
    VARIABLE,
    FUNCTION,
    CLASS
} kind_t;

/* the type of identifier */
typedef enum type_enum {
    VOID,
    CHAR,
    INT,
    FLOAT,
    DOUBLE,
    POINTER_VOID,
    POINTER_INT,
    POINTER_FLOAT,
    POINTER_CHAR,
    POINTER_DOUBLE
} type_t;

/* the symbol table entry */
typedef struct entry_struct {
    char *identifier;
    type_t type;
    kind_t kind;
    struct entry_struct *next;
} entry_t;

/* the symbol table structure */
typedef struct symbol_table_struct {
    struct symbol_table_struct *parent;
    entry_t *entry_top;
} symbol_table_t;



/* function declarations */
char* get_token(FILE* file);

entry_t* symbol_table_insert(symbol_table_t* symbol_table, entry_t entry);
entry_t* symbol_table_lookup(symbol_table_t* symbol_table, char* name);
symbol_table_t* symbol_table_enter_scope(symbol_table_t* symbol_table);
symbol_table_t* symbol_table_exit_scope(symbol_table_t* symbol_table);



int main(int argc, char *argv[])
{
    int is_eof = 0;
    int is_identifier = 0;
    int num_of_errors = 0;
    char *token = NULL;
    FILE *source = NULL;
    symbol_table_t *table_root = NULL;
    symbol_table_t *table = NULL;
    entry_t entry;

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

    /* Initialize the root of the table */
    table_root = symbol_table_enter_scope(NULL);
    table = table_root;


    /* Loop through the file till eof */
    while (!feof(source)) {

        /* get a token */
        token = get_token(source);
        if (token == NULL) {
            break;
        }

        /* if it's a keyword, ignore */
        if (strcmp(token, "while") == 0
                || strcmp(token, "if") == 0
                || strcmp(token, "do") == 0
                || strcmp(token, "else") == 0
                || strcmp(token, "return") == 0
                || strcmp(token, "sizeof") == 0
                || strcmp(token, "NULL") == 0
                || strcmp(token, "break") == 0
                || strcmp(token, "continue") == 0
                || strcmp(token, "for") == 0) {
            continue;
        }

        /* if it's a declaration/definition, add it as an entry */
        is_identifier = 1;
        if (strcmp(token, "void") == 0) {
            entry.type = VOID;
        } else if (strcmp(token, "char") == 0) {
            entry.type = CHAR;
        } else if (strcmp(token, "int") == 0) {
            entry.type = INT;
        } else if (strcmp(token, "float") == 0) {
            entry.type = FLOAT;
        } else if (strcmp(token, "double") == 0) {
            entry.type = DOUBLE;
        } else {
            is_identifier = 0;
        }

        if (is_identifier) {
            int is_pointer = 0;

            token = get_token(source);
            if (token[0] == '*') {
                is_pointer = 1;
                entry.type = entry.type += POINTER_VOID - VOID;
                token = get_token(source);
            }

            entry.identifier = token;
            
            /* check if there's a identifier with the same name in the local
             * scope */
            scope_level = 0;
            if (symbol_table_lookup(table, token) && !scope_level) {
                fprintf(stderr, "'%s' is already defined/declared\n", token);
                num_of_errors++;
            }
            symbol_table_insert(table, entry);
            continue;
        }

        /* if it's a '{' start a new scope */
        if (strcmp(token, "{") == 0) {
            if (!is_func) {
                table = symbol_table_enter_scope(table);
                is_func = 0;
            }
            continue;
        }

        /* if it's a '}' end the current scope */
        if (strcmp(token, "}") == 0) {
            table = symbol_table_exit_scope(table);
            continue;
        }

        /* if it's a '(' following entries are params */
        if (strcmp(token, "(") == 0) {

            /* incrementing is_param rather than equating to 0 so that brackets
             * can be accepted in the arguments for resolution */
            is_param++;

            /* parameters are in the scope of the function, so enter a new
             * scope and ignore the next '{' */
            table = symbol_table_enter_scope(table);
            is_func = 1;
            continue;
        }

        /* if it's a ')' following entries are not params */
        if (strcmp(token, ")") == 0) {
            is_param--;
            continue;
        }

        /* if it's a '[' or ']', ignore (for now) */
        if (strcmp(token, "[") == 0 || strcmp(token, "]") == 0) {
            continue;
        }

        /* if it's a use, lookup in the symbol table */
        if (token[0] > 'A' && token[0] < 'z') {
            if(symbol_table_lookup(table, token) == NULL) {
                fprintf(stderr, "Could not find '%s' in scope\n", token);
                num_of_errors++;
            }
        }

    }

    /* delete root table and free memory */
    table = symbol_table_exit_scope(table);


    /* Close the file */
    fclose(source);

    if (num_of_errors) {
        fprintf(stderr, "\nThe program found %d errors\n", num_of_errors);
        exit(1);
    }

    fprintf(stderr, "No errors in scope found.\n");
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

    while (i < MAX_BUFFER_SIZE && !feof(file)) {

        /* read a character from the file */
        if (!fscanf(file, "%c", &buf)) {
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
        /* if it's a comment, ignore */
        else if (buf[0] == '/') {
            char temp[200];
            fscanf(file, "%c", &temp);
            /* it's a single line comment */
            if (temp[0] == '/') {
                while (!feof(file)) {
                    fscanf(file, "%c", &temp);
                    if (temp[0] == 10) {
                        break;
                    }
                }
                i = 0;
                continue;
            /* it's a multiline comment */
            } else if (temp[0] == '*') {
                while (!feof(file) && fscanf(file, "%c", &temp)) {
                    if (temp[0] == '*'
                            && fscanf(file, "%c", &temp)
                            && temp[0] == '/') {
                        break;
                    }
                }
                i = 0;
                continue;
            /* not a comment */
            } else {
                buf[0] = '/';
                fseek(file, -1, SEEK_CUR);
                break;
            }
            fscanf(file, "%s", &temp);
            fscanf(file, "%s", &temp);
        }
        /* if it's a preprocessor directive, ignore */
        else if (buf[0] == '#') {
            char temp[200];
            fscanf(file, "%s", &temp);
            fscanf(file, "%s", &temp);
        }
        /* if it's a string, send as a whole */
        else if (buf[0] == '"') {
            temp_token[i] = buf[0];
            i++;

            /* scan till the matching quotes */
            while (!feof(file)) {
                fscanf(file, "%c", &buf);

                if (buf[0] == '\\') {
                    /* take care of escape sequence */
                    fscanf(file, "%c", &buf);
                } else if (buf[0] == '"') {
                    break;
                } else {
                    temp_token[i] = buf[0];
                    i++;
                }
            }

            temp_token[i] = buf[0];
            i++;
            break;
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
 * Insert entry if not already in the current scope. Error if a symbole of the
 * same name is already present in the current scope
 *
 * @param symbol_table_t The symbol table to search in
 * @param entry_t The entry to add to the table
 * @return entry_t Pointer to the new entry. NULL if error
 */
entry_t* symbol_table_insert(symbol_table_t* symbol_table, entry_t entry)
{
    entry_t *new_entry = NULL;
    entry_t *cursor = NULL;

    new_entry = (entry_t *) malloc(sizeof(entry));
    if (new_entry == NULL) {
        return NULL;
    }
    new_entry -> type = entry.type;
    new_entry -> kind = entry.kind;
    new_entry -> identifier = entry.identifier;
    new_entry -> next = NULL;

    cursor = symbol_table -> entry_top;

    /* if it's the first entry, insert and return */
    if (cursor == NULL) {
        symbol_table -> entry_top = new_entry;
        return new_entry;
    }

    /* go to the last entry and insert */
    while (cursor -> next != NULL) {
        cursor = cursor -> next;
    }

    cursor -> next = new_entry;
    return new_entry;
}

/**
 * Search the symbol table for the given indentifier name and return a pointer
 * to the latest entry.
 *
 * @param symbol_table_t The pointer to the symbol table to search in
 * @param char* The name of the identifier
 * @return entry_t* The pointer to the symbol table entry. NULL if not found.
 */
entry_t* symbol_table_lookup(symbol_table_t* symbol_table, char* name)
{
    symbol_table_t *table = NULL;
    entry_t *entry = NULL;

    if (symbol_table == NULL) {
        return NULL;
    }

    table = symbol_table;
    entry = symbol_table -> entry_top;

    while (entry != NULL) {
        if (strcmp(entry -> identifier, name) == 0) {
            return entry;
        }
        entry = entry -> next;
    }

    /* increment the scope level if searching in parent */
    scope_level++;

    return symbol_table_lookup(table -> parent, name);
}

/**
 * Create a new entry in the given symbol table and return a pointer to it
 *
 * @param symbol_table_t* The pointer to the symbol table to create entry into
 * @return symbol_table_t* The pointer to the new symbol table. NULL if error
 */
symbol_table_t* symbol_table_enter_scope(symbol_table_t* symbol_table)
{
    symbol_table_t *new_table = NULL;

    new_table = (symbol_table_t *) malloc(sizeof(symbol_table_t));
    new_table -> parent = symbol_table;
    new_table -> entry_top = NULL;

    return new_table;
}

/**
 * Return the parent symbol table by exiting the scope of the current one
 *
 * @param symbol_table_t* The pointer to the current symbol table
 * @return symbol_table_t* The pointer to the parent symbol table. NULL if not
 *      found
 */
symbol_table_t* symbol_table_exit_scope(symbol_table_t* symbol_table)
{
    symbol_table_t *parent;

    parent = symbol_table -> parent;
    free(symbol_table);

    return parent;
}
