#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "symbol.h"
#include "ast.h"

symtable *sym_table = symtable_init();
int scope_num = 0;

void preDefVarIntoSymTable(void) {

    // insert pre-defined variables into symtable
    insert_into_symtable(sym_table, "gl_FragColor", VEC4, RESULT, scope_num);
    insert_into_symtable(sym_table, "gl_FragDepth", BOOL, RESULT, scope_num);
    insert_into_symtable(sym_table, "gl_FragCoord", VEC4, RESULT, scope_num);
    insert_into_symtable(sym_table, "gl_TexCoord", VEC4, ATTRIBUTE, scope_num);
    insert_into_symtable(sym_table, "gl_Color", VEC4, ATTRIBUTE, scope_num);
    insert_into_symtable(sym_table, "gl_Secondary", VEC4, ATTRIBUTE, scope_num);
    insert_into_symtable(sym_table, "gl_FogFragCoord", VEC4, ATTRIBUTE, scope_num);
    insert_into_symtable(sym_table, "gl_Light_Half", VEC4, UNIFORM, scope_num);
    insert_into_symtable(sym_table, "gl_Light_Ambient", VEC4, UNIFORM, scope_num);
    insert_into_symtable(sym_table, "gl_Material_Shininess", VEC4, UNIFORM, scope_num);
    insert_into_symtable(sym_table, "env1", VEC4, UNIFORM, scope_num);
    insert_into_symtable(sym_table, "env2", VEC4, UNIFORM, scope_num);
    insert_into_symtable(sym_table, "env3", VEC4, UNIFORM, scope_num);
}
    
void symbol_table(node *ast) {
    if(!ast)
        return;

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

            insert_into_symtable(sym_table, ast->type_declaration.id, ast->type_declaration.type->type.type_kind, NONCONST, scope_num);
            break;

        case 5:
            symbol_table(ast->assign_declaration.type);
            symbol_table(ast->assign_declaration.expression);

            insert_into_symtable(sym_table, ast->assign_declaration.id, ast->assign_declaration.type->type.type_kind, NONCONST, scope_num);
            break;

        case 6:
            symbol_table(ast->const_declaration.type);
            symbol_table(ast->const_declaration.expression);

            insert_into_symtable(sym_table, ast->const_declaration.id, ast->const_declaration.type->type.type_kind, _CONST, scope_num); 
            break;

        case 7:
            symbol_table(ast->assign_statement.variable);
            symbol_table(ast->assign_statement.expression);
            break;

        case 8:
            symbol_table(ast->if_statement.condition);
            symbol_table(ast->if_statement.statement);
            break;

        case 9:
            symbol_table(ast->if_else_statement.condition);
            symbol_table(ast->if_else_statement.statement);
            symbol_table(ast->if_else_statement.else_statement);
            break;

        case 10:
            break;

        case 11:
            symbol_table(ast->unary_expr.right);
            break;

        case 12:
            symbol_table(ast->binary_expr.left);
            symbol_table(ast->binary_expr.right);
            break;

        case 13:
            symbol_table(ast->brackets_expr.expression);
            break;

        case 14:
            symbol_table(ast->func_expr.arguments_opt);

            switch(ast->func_expr.func) {
                case 0:
                    insert_into_symtable(sym_table, "dp3", FUNCTION, NONCONST, scope_num);                                
                    break;
                case 1:
                    insert_into_symtable(sym_table, "lit", FUNCTION, NONCONST, scope_num);                                                                
                    break;
                case 2:
                    insert_into_symtable(sym_table, "rsq", FUNCTION, NONCONST, scope_num);
                    break;
                default:
                    break;
            }
            break;

        case 15:
            symbol_table(ast->type_expr.type);
            symbol_table(ast->type_expr.arguments_opt);
            break;

        case 16:
            symbol_table(ast->var_expr.variable);
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

        case 25:
            break;
            
        case 26:
            scope_num++;
            symbol_table(ast->prog_scope.scope);
            scope_num--;
            break;

        default:
            break;
    }
}
