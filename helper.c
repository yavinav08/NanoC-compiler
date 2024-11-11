/* header files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"

#define NOT_INIT -10000000

/* declaration of useful functions and variables */
/**
 * @param install_id to add the symbol in the hash table.
 * @param install_int to add an integer in the hash table.
 * @param install_real to add a real number in the hash table.
 */
char *install_id();
int install_int();
float install_real();
int line = 0;

/**
 * @brief linked list implementation
 */
typedef struct node
{
    char *specifier;
    struct node *next;
} node;

/**
 * @param head is the linked list of all nodes to check duplicates
 */
node *head = NULL;

/**
 * @brief to search a specifier in the nodes linked list
 */
int search_id_in_list(char *specifier)
{
    node *tmp = head;
    while (tmp != NULL && strcmp(specifier, tmp->specifier))
    {
        tmp = tmp->next;
    }
    // return 0 if not found, 1 if found
    return (tmp != NULL);
}

/**
 * @brief to add a new id to the beggining of the list
 */
int add_id_to_list(char *specifier)
{
    node *tmp = (node *)malloc(sizeof(node));
    tmp->specifier = (char *)strdup(specifier);
    tmp->next = NULL;

    // do not add if the id is already present
    if (search_id_in_list(specifier))
    {
        return 0;
    }

    if (head)
    {
        tmp->next = head;
        head = tmp;
    }
    else
        head = tmp;

    return 1;
}

/**
 * @brief function to print the linked list
 */
void print_id_list()
{
    node *tmp = head;
    while (tmp != NULL)
    {
        printf("%s -> ", tmp->specifier);
        tmp = tmp->next;
    }
    printf("NULL\n");
}

/* hashtable implementation */
/* Defining the size of the hash table */
#define SIZE 20

/* The symbol struct represents a symbol, containing a specifier, a type, whether number is an integer or real number and a pointer to the next symbol in the bucket */
typedef struct symbol
{
    char *specifier;
    float real;
    int integer;
    char type;
    int initialized;
    int declared;
    struct symbol *next;
} symbol;

symbol *SYMTAB[SIZE];

/**
 * @brief a hash function which is just the sum of the ascii values of the characters of specifier modulo SIZE. */
int hash_function(char *specifier)
{
    int len = strlen(specifier);
    // summing the ascii values
    int hash = 0;
    for (int i = 0; i < len; i++)
        hash += (int)specifier[i];
    return hash % SIZE;
}

/**
 * @brief Initialize all buckets of the hashtable with null */
void init()
{
    for (int i = 0; i < SIZE; i++)
        SYMTAB[i] = NULL;
}
/**
 * @brief Function to insert a symbol in hashtable
 */
symbol *insert(char *specifier, char type)
{
    // compute the hash function to get index
    int i = hash_function(specifier);

    // create the new symbol
    symbol *symbol_to_insert = (symbol *)malloc(sizeof(symbol));
    symbol_to_insert->specifier = (char *)strdup(specifier);
    symbol_to_insert->real = NOT_INIT;

    symbol_to_insert->integer = NOT_INIT;
    symbol_to_insert->type = type;
    symbol_to_insert->initialized = 0;
    symbol_to_insert->declared = 0;

    // insert it at the beginning of the bucket linked list
    symbol_to_insert->next = SYMTAB[i];
    SYMTAB[i] = symbol_to_insert;

    return symbol_to_insert;
}

/**
 * @brief Function to search a symbol in hashtable
 */
symbol *search_in_hash_table(char *specifier)
{
    // compute the hash function
    int i = hash_function(specifier);

    // search the linked list of with the bucket
    symbol *tmp = SYMTAB[i];
    while (tmp != NULL && strcmp(tmp->specifier, specifier))
        tmp = tmp->next;

    return tmp;
}

/**
 * @brief to print the hashtable
 */
void print_hash_table(FILE *file)
{
    // pretty printing of hash table
    fprintf(file, "-------------------------------------------------------------------------------------\n");

    for (int i = 0; i < SIZE; i++)
    {
        fprintf(file, "bucket [%d]: ", i);
        symbol *tmp = SYMTAB[i];
        while (tmp != NULL)
        {
            fprintf(file, "<%s %c : ", tmp->specifier, tmp->type);
            fprintf(file, "{ ");
            fprintf(file, "declared: %d, initialized: %d ", tmp->declared, tmp->initialized);
            fprintf(file, "}> -> ");
            tmp = tmp->next;
        }
        fprintf(file, "NULL\n");
    }
    fprintf(file, "-------------------------------------------------------------------------------------\n");
}

/**
 * @brief returns the current line number to the parser
 */
int get_line_number()
{
    return line;
}

/**
 * @brief to search the symbol and get the int value of the symbol
 */
int get_int_value(char *specifier)
{
    symbol *sym = search_in_hash_table(specifier);
    if (sym != NULL)
        return sym->integer;

    return 0;
}
/**
 * @brief to search the symbol and get the float value of the symbol
 */
float get_real_value(char *specifier)
{
    symbol *sym = search_in_hash_table(specifier);
    if (sym != NULL)
        return sym->real;

    return 0.0;
}

/**
 * @brief to search the symbol and set its value to val
 */
void set_int_value(char *specifier, int val)
{
    symbol *sym = search_in_hash_table(specifier);
    if (sym != NULL)
    {
        sym->integer = val;
        sym->type = 'I';
        sym->initialized = 1;
    }
}

/**
 * @brief to search the symbol and set its value to val
 */
void set_real_value(char *specifier, float val)
{
    symbol *sym = search_in_hash_table(specifier);
    if (sym != NULL)
    {
        sym->real = val;
        sym->type = 'F';
        sym->initialized = 1;
    }
}

/**
 * @brief a function to check if the specifier is declared or not
 */
int is_declared(char *specifier)
{
    symbol *sym = search_in_hash_table(specifier);
    if (sym != NULL)
        return (sym->declared);

    return 0;
}

/**
 * @brief a function to check if the specifier is initialised or not
 */
int is_initialized(char *specifier)
{
    symbol *sym = search_in_hash_table(specifier);
    if (sym != NULL)
    {
        return (sym->initialized);
    }

    return 0;
}

/**
 * @brief to search the symbol and get its type
 */
int get_type(char *specifier)
{
    symbol *sym = search_in_hash_table(specifier);
    if (sym != NULL)
    {
        if (sym->type == 'I')
            return 0;
        if (sym->type == 'F')
            return 1;
        if (sym->type == 'V')
            return 2;
    }
    return 0;
}
/**
 * @brief This function is to set the declaration of the variables and set their type
 */
int set_type(int type)
{
    node *tmp = head;
    int flag = 1;
    while (tmp != NULL)
    {
        symbol *sym = search_in_hash_table(tmp->specifier);
        if (sym->declared)
            flag = 0; // duplicate error
        else
        {
            if (type == 0)
                sym->type = 'I';
            if (type == 1)
                sym->type = 'F';
            sym->declared = 1;
        }
        tmp = tmp->next;
    }
    // set nodes list as empty
    head = NULL;
    return flag;
}

/**
 * @brief a function to initialize all the variables in the id list while reading
 */
int set_init()
{
    node *tmp = head;
    int flag = 1;
    while (tmp != NULL)
    {
        symbol *sym = search_in_hash_table(tmp->specifier);
        if (sym->declared == 0) // missing declaration
            flag = 0;
        else
            sym->initialized = 1;
        tmp = tmp->next;
    }
    head = NULL;
    return flag;
}
int set_initt()
{
    node *tmp = head;
    int flag = 1;
    while (tmp != NULL)
    {
        symbol *sym = search_in_hash_table(tmp->specifier);
        // printf("%s",sym->specifier);
        if (sym->declared == 0) // missing declaration
            flag = 0;
        else
            sym->initialized = 1;
        tmp = tmp->next;
    }
    head = NULL;
    return flag;
}
/**
 * @brief function to clear the linked list
 */
void clear_list()
{
    head = NULL;
}
