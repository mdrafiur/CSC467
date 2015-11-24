#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct symtable_node_t symtable_node;

struct symtable_node_t {
    char *name;
    int dtype;
    int scope;
    struct symtable_node_t *next;
};

struct symtable_t {
    int num_item;
    struct symtable_node_t *head;
};


