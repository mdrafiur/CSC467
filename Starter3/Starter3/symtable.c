#include "symtable.h"

struct symtable_t *symtable;

struct symtable_t *symtable_init(void)
{
    symtable = malloc(sizeof(struct symtable_t));
    assert(symtable);

    symtable->num_item = 0;
    symtable->head = NULL;
              
    return symtable;
}

void insert_into_symtable(char *sym_name, int type, int scope) {
            
    symtable_node *new_node;
                
    if(lookup_symtable(sym_name))
        return;

    new_node = (symtable_node *)malloc(sizeof(symtable_node));
    assert(new_node);
                                                        
    new_node->name = (const char *)malloc(strlen(name) + 1);
    assert(new_node->name);
    strcpy(new_node->name, sym_name);
    new_node->dtype = type;
    new_node->scope = scope;

    new_node->next = symtable->head;
    symtable->head = new_node;
    symtable->num_item++;
    return;
    
}

bool lookup_symtable(const char *name)
{
    symtable_node *current;
    current = symtable->head;
    assert(name);

    while(current) {
        if(strcmp(current->sym_name, name) == 0)
            return true;

        current = current->next;
    }

    return false;
}

int scope_check(char *name, int scope)
{
    symtable_node *current;
    current = symtable->head;
    assert(name);

    while(current) {
        if(strcmp(current->sym_name, name) == 0 && current->scope == scope)
            return current->dtype;

        current = current->next;
    }

    return -1;
}

int remove_from_symtable(char *sym_name)
{
    assert(name);

    symtable_node *current = symtable->head;
    symtable_node *prev = NULL;

    for (current = symtable->head, prev = NULL; current; prev = current, current = current->next)
    {
        if (strcmp(current->name, sym_name) == 0) {

            if (!prev)
                symtable->head = symtable->head->next;
            else
                prev->next = current->next;
                                                                      
            free(current->name);
            free(current);
            symtable->num_item--;
            return 0;
        }
    }
    return -1;
}

