#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "symbol.h"
#include "ast.h"
#include "symtable.h"

void add_symtable(node *ast) {

    assert(ast);

    switch((int)ast->kind){
        case 0:
            break;

        case 1:
            add_symtable(ast->scope.declarations);
            add_symtable(ast->scope.statements);
            break;

        case 2:
            add_symtable(ast->declarations.declarations);
            add_symtable(ast->declarations.declaration);
            break;

        case 3:
            add_symtable(ast->statements.statements);
            add_symtable(ast->statements.statement);
            break;

        case 4:
            add_symtable(ast->type_declaration.type);
            add_symtable(ast->type_declaration.id);
            break;

        case 5:
            add_symtable(ast->assign_declaration.type);
            add_symtable(ast->assign_declaration.id);
            add_symtable(ast->assign_declaration.expression);
            break;

        case 6:
            add_symtable(ast->const_declaration.type);
            add_symtable(ast->const_declaration.id);
            add_symtable(ast->const_declaration.expression);
            break;

        case 7:
            add_symtable(ast->assign_statement.variable);
            add_symtable(ast->assign_statement.expression);
            break;
        case 9:
            break;
        case 10:
            break;
        case 11:
            break;
        case 12:
            break;
        case 13:
            break;
        case 14:
            break;
