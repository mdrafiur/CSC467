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
            
            insert_into_symtable(ast->type_declaration.id, -ast->type_declaration.type->type.type_kind, scope_num);
            break;

        case 5:
            add_symtable(ast->assign_declaration.type);
            add_symtable(ast->assign_declaration.expression);

            insert_into_symtable(ast->assign_declaration.id, ast->assign_declaration.type->type.type_kind, scope_num);
            break;

        case 6:
            add_symtable(ast->const_declaration.type);
            add_symtable(ast->const_declaration.expression);

            insert_into_symtable(ast->const_declaration.id, ast->const_declaration.type->type.type_kind, scope_num); 
            break;

        case 7:
            add_symtable(ast->assign_statement.variable);
            add_symtable(ast->assign_statement.expression);
            break;

        case 8:
            add_symtable(ast->if_statement.codition);
            add_symtable(ast->if_statement.statement);
            break;

        case 9:
            add_symtable(ast->if_else_statement.condition);
            add_symtable(ast->if_else_statement.statement);
            add_symtable(ast->if_else_statement.else_statement);
            break;

        case 10:
            scope_num++;
            add_symtable(ast->scope_statement.scope);
            scope_num--;
            break;

        case 11:
            add_symtable(ast->unary_expr.op);
            add_symtable(ast->unary_expr.right);
            break;

        case 12:
            add_symtable(ast->binary_expr.op);
            add_symtable(ast->binary_expr.left);
            add_symtable(ast->binary_expr.right);
            break;

        case 13:
            add_symtable(ast->brackets_expr.expression);
            break;

        case 14:
            add_symtable(ast->func_expr.func);
            add_symtable(ast->func_expr.arguments_opt);

            if(ast->func_exp.func == 0){
                insert_into_symtable("dp3", FUNCTION, scope_num);                                
            }
            else if(ast->func_exp.func == 1){
                insert_into_symtable("lit", FUNCTION, scope_num);                                                                
            }
            else if (ast->func_exp.func == 2){
                insert_into_symtable("rsq", FUNCTION, scope_num);
            }
            break;

        case 15:
            add_symtable(ast->type_expr.type);
            add_symtable(ast->type_expr.arguments_opt);
            break;

        case 16:
            break;

        case 17:
            break;

        case 18:
            break;

        case 19:
            break;

        case 20:
            break;

        case 21:
            break;

        case 22:
            add_symtable(ast->args_arguments.arguments);
            add_symtable(ast->args_arguments.expression);
            break;

        case 23:
            add_symtable(ast->expr_arguments.expression);
            break;

        case 24:
            add_symtable(ast->arguments_opt.arguments);
            break;

        case 25;
            break;

        default
            break;

                                                   break;
