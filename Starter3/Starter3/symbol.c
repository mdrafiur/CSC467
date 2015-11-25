#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "symbol.h"
#include "ast.h"
#include "symtable.h"


void symbol_table(node *ast) {

    assert(ast);

    // insert pre-defined variables into symtable
    insert_into_symtable("gl_FragColor", VEC4, RESULT, 0);
    insert_into_symtable("gl_FragDepth", BOOL, RESULT, 0);
    insert_into_symtable("gl_FragCoord", VEC4, RESULT, 0);
    insert_into_symtable("gl_TexCoord", VEC4, ATTRIBUTE, 0);
    insert_into_symtable("gl_Color", VEC4, ATTRIBUTE, 0);
    insert_into_symtable("gl_Secondary", VEC4, ATTRIBUTE, 0);
    insert_into_symtable("gl_FogFragCoord", VEC4, ATTRIBUTE, 0);
    insert_into_symtable("gl_Light_Half", VEC4, UNIFORM, 0);
    insert_into_symtable("gl_Light_Ambient", VEC4, UNIFORM, 0);
    insert_into_symtable("gl_Material_Shininess", VEC4, UNIFORM, 0);
    insert_into_symtable("env1", VEC4, UNIFORM, 0);
    insert_into_symtable("env2", VEC4, UNIFORM, 0);
    insert_into_symtable("env3", VEC4, UNIFORM, 0);

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

            insert_into_symtable(ast->type_declaration.id, ast->type_declaration.type->type.type_kind, NONCONST, scope_num);
            break;

        case 5:
            symbol_table(ast->assign_declaration.type);
            symbol_table(ast->assign_declaration.expression);

            insert_into_symtable(ast->assign_declaration.id, ast->assign_declaration.type->type.type_kind, NONCONST, scope_num);
            break;

        case 6:
            symbol_table(ast->const_declaration.type);
            symbol_table(ast->const_declaration.expression);

            insert_into_symtable(ast->const_declaration.id, ast->const_declaration.type->type.type_kind, CONST, scope_num); 
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
            symbol_table(ast->func_expr.arguments_opt);

            switch(ast->func_exp.func) {
                case 0:
                    insert_into_symtable("dp3", FUNCTION, NONCONST, scope_num);                                
                    break;
                case 1:
                    insert_into_symtable("lit", FUNCTION, NONCONST, scope_num);                                                                
                    break;
                case 2:
                    insert_into_symtable("rsq", FUNCTION, NONCONST, scope_num);
                    break;
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
