#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "symbol.h"
#include "ast.h"
#include "symtable.h"

void symbol_table(node *ast) {

    assert(ast);

    switch((int)ast->kind){
        case 0:
            break;

        case 1:
            symbol_table(ast->scope.declarations);
            symbol_table(ast->scope.statements);
            break;

        case 2:
            symbol_table(ast->declarations.declarations);
            symbol_table(ast->declarations.declaration);
            break;

        case 3:
            symbol_table(ast->statements.statements);
            symbol_table(ast->statements.statement);
            break;

        case 4:
            symbol_table(ast->type_declaration.type);
            
            insert_into_symtable(ast->type_declaration.id, -ast->type_declaration.type->type.type_kind, scope_num);
            break;

        case 5:
            symbol_table(ast->assign_declaration.type);
            symbol_table(ast->assign_declaration.expression);

            insert_into_symtable(ast->assign_declaration.id, ast->assign_declaration.type->type.type_kind, scope_num);
            break;

        case 6:
            symbol_table(ast->const_declaration.type);
            symbol_table(ast->const_declaration.expression);

            insert_into_symtable(ast->const_declaration.id, ast->const_declaration.type->type.type_kind, scope_num); 
            break;

        case 7:
            symbol_table(ast->assign_statement.variable);
            symbol_table(ast->assign_statement.expression);
            break;

        case 8:
            symbol_table(ast->if_statement.codition);
            symbol_table(ast->if_statement.statement);
            break;

        case 9:
            symbol_table(ast->if_else_statement.condition);
            symbol_table(ast->if_else_statement.statement);
            symbol_table(ast->if_else_statement.else_statement);
            break;

        case 10:
            scope_num++;
            symbol_table(ast->scope_statement.scope);
            scope_num--;
            break;

        case 11:
            symbol_table(ast->unary_expr.op);
            symbol_table(ast->unary_expr.right);
            break;

        case 12:
            symbol_table(ast->binary_expr.op);
            symbol_table(ast->binary_expr.left);
            symbol_table(ast->binary_expr.right);
            break;

        case 13:
            symbol_table(ast->brackets_expr.expression);
            break;

        case 14:
            symbol_table(ast->func_expr.func);
            symbol_table(ast->func_expr.arguments_opt);

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
            symbol_table(ast->type_expr.type);
            symbol_table(ast->type_expr.arguments_opt);
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
            symbol_table(ast->args_arguments.arguments);
            symbol_table(ast->args_arguments.expression);
            break;

        case 23:
            symbol_table(ast->expr_arguments.expression);
            break;

        case 24:
            symbol_table(ast->arguments_opt.arguments);
            break;

        case 25;
            break;

        default
            break;

                                                   break;
