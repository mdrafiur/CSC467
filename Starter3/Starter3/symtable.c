#include "symtable.h"


symtable *symtable_init(void)
{
    symtable *s = (symtable *)malloc(sizeof(symtable));
    assert(s);

    s->num_item = 0;
    s->head = NULL;
              
    return s;
}

void insert_into_symtable(symtable *sym_table, char *sym_name, int type, int tClass, int scope) 
{            
    symtable_node *new_node;
                
    if(lookup_symtable(sym_table, sym_name))
        return;

    new_node = (symtable_node *)malloc(sizeof(symtable_node));
    assert(new_node);
                                                        
    new_node->name = (char *)malloc(strlen(sym_name) + 1);
    assert(new_node->name);
    strcpy(new_node->name, sym_name);
    new_node->dtype = type;
    new_node->tClass = tClass;
    new_node->scope = scope;

    new_node->next = sym_table->head;
    sym_table->head = new_node;
    sym_table->num_item++;
    return;
}

bool lookup_symtable(symtable *sym_table, char *name)
{
    symtable_node *current;
    current = sym_table->head;
    assert(name);

    while(current) {
        if(strcmp(current->name, name) == 0)
            return true;

        current = current->next;
    }

    return false;
}

int scope_check(symtable *sym_table, char *name, int scope)
{
    symtable_node *current;
    current = sym_table->head;
    assert(name);

    while(current) {
        if(strcmp(current->name, name) == 0 && current->scope == scope)
            return current->dtype;

        current = current->next;
    }

    return -1;
}

int get_data_type (symtable *sym_table, char *name)
{
    symtable_node *current;
    current = sym_table->head;
    
    assert(name);
               
    while(current) {
        if(strcmp(current->name, name) == 0)
            return current->dtype;
         
        current = current->next;
    }

    return -1;
}

int get_tClass (symtable *sym_table, char *name)
{
    symtable_node *current;
    current = sym_table->head;
    
    assert(name);
               
    while(current) {
        if(strcmp(current->name, name) == 0)
            return current->tClass;
         
        current = current->next;
    }

    return -1;
}

int remove_from_symtable(symtable *sym_table, char *sym_name)
{
    assert(sym_name);
    symtable_node *current;
    symtable_node *prev;

    for (current = sym_table->head, prev = NULL; current; prev = current, current = current->next)
    {
        if (strcmp(current->name, sym_name) == 0) {

            if (!prev)
                sym_table->head = sym_table->head->next;
            else
                prev->next = current->next;
                                                                      
            free(current->name);
            free(current);
            sym_table->num_item--;
            return 0;
        }
    }
    return -1;
}

