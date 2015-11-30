#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef struct symtable_node_t symtable_node;
typedef struct symtable_t symtable;
extern symtable *sym_table;

typedef enum {
	_CONST,
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
void insert_into_symtable(symtable *sym_table, char *sym_name, int type, int tClass, int scope);
bool lookup_symtable(symtable *sym_table, char *name);
bool isVarDeclared(symtable *sym_table, char *name, int scope);
bool isVarDeclaredInScope(symtable *sym_table, char *name, int scope);
int get_data_type (symtable *sym_table, char *name);
int get_tClass (symtable *sym_table, char *name);
int remove_from_symtable(symtable *sym_table, char *sym_name);
