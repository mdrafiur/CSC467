#include <assert.h>
#include "symtable.h"

symtable *symtable_init(void)
{
    symtable *head = (symtable *)malloc(sizeof(symtable));
    assert(head);

    head->num_item = 0;
    head->head = NULL;
              
    return head;
}

void insert_into_symtable(char *sym_name, int type, int tClass, int scope) {
            
    symtable_node *new_node;
                
    if(lookup_symtable(sym_name))
        return;

    new_node = (symtable_node *)malloc(sizeof(symtable_node));
    assert(new_node);
                                                        
    new_node->name = malloc(strlen(sym_name) + 1);
    assert(new_node->name);
    strcpy(new_node->name, sym_name);
    new_node->dtype = type;
    new_node->tClass = tClass;
    new_node->scope = scope;

    new_node->next = head->head;
    head->head = new_node;
    head->num_item++;
    return;
    
}

bool lookup_symtable(char *name)
{
    symtable_node *current;
    current = head->head;
    assert(name);

    while(current) {
        if(strcmp(current->name, name) == 0)
            return true;

        current = current->next;
    }

    return false;
}

int scope_check(char *name, int scope)
{
    symtable_node *current;
    current = head->head;
    assert(name);

    while(current) {
        if(strcmp(current->name, name) == 0 && current->scope == scope)
            return current->dtype;

        current = current->next;
    }

    return -1;
}

int get_data_type (char *name)
{
    symtable_node *current;
    current = head->head;
    
    assert(name);
               
    while(current) {
        if(strcmp(current->name, name) == 0)
            return current->dtype;
         
        current = current->next;
    }

    return -1;
}

int get_tClass (char *name)
{
    symtable_node *current;
    current = head->head;
    
    assert(name);
               
    while(current) {
        if(strcmp(current->name, name) == 0)
            return current->tClass;
         
        current = current->next;
    }

    return -1;
}

int remove_from_symtable(char *sym_name)
{
    assert(sym_name);
    symtable_node *current;
    symtable_node *prev;

    for (current = head->head, prev = NULL; current; prev = current, current = current->next)
    {
        if (strcmp(current->name, sym_name) == 0) {

            if (!prev)
                head->head = head->head->next;
            else
                prev->next = current->next;
                                                                      
            free(current->name);
            free(current);
            head->num_item--;
            return 0;
        }
    }
    return -1;
}

