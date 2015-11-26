#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct symtable_node_t symtable_node;
typedef struct symtable_t symtable;
symtable *head;

typedef enum {
	CONST,
	NONCONST,
	ATTRIBUTE,
	UNIFORM,
	RESULT,
} tClass;

struct symtable_node_t {
    char *name;
    int dtype;
    int tClass;
    int scope;
    struct symtable_node_t *next;
};

struct symtable_t {
    int num_item;
    struct symtable_node_t *head;
};

symtable *symtable_init(void);
void insert_into_symtable(char *sym_name, int type, int tClass, int scope);
bool lookup_symtable(const char *name);
int scope_check(const char *name, int scope);
int get_data_type (const char *name);
int get_tClass (const char *name);
int remove_from_symtable(char *sym_name);
