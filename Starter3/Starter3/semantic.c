#include "semantic.h"

int ct_scope = 0;
int r_expr, l_expr;
int type_class;

int semantic_check( node *ast) {

    assert(ast);

    switch((int)ast->kind){
        case 0:
            break;

        case 1:
            r_expr = semantic_check(ast->scope.declarations);
            l_expr = semantic_check(ast->scope.statements);

            if(r_expr == -1 || l_expr == -1)
                return -1;
            else
                return 0;
            break;

        case 2:
            l_expr = semantic_check(ast->declarations.declaration);
            r_expr = semantic_check(ast->declarations.declarations);

            if(r_expr == -1 || l_expr == -1)
                return -1;
            else
                return l_expr;
            break;

        case 3:
            l_expr = semantic_check(ast->statements.statement);
            r_expr = semantic_check(ast->statements.statements);

            if(r_expr == -1 || l_expr == -1)
                return -1;
            else
                return l_expr;
            break;

        case 4:
            if(scope_check(ast->type_declaration.id, ct_scope) == -1) {
                fprintf(errorFile, "Error: Variable cannot be re-declared within the same scope\n");
                errorOccurred = 1;    
                return -1;
            }
            else
                return semantic_check(ast->type_declaration.type);
                                                                        
            break;
            
        case 5:
            l_expr = semantic_check(ast->assign_declaration.type);
            r_expr = semantic_check(ast->assign_declaration.expression);

            if(r_expr == -1 || l_expr == -1)
                return -1;

            if(scope_check(ast->assign_declaration.id, ct_scope) == -1) {
                fprintf(errorFile, "Error: Variable cannot be re-declared within the same scope\n");
                errorOccurred = 1;
                return -1;                                                            
            }

            if(l_expr != r_expr){
                fprintf(errorFile, "Error: Type mismatch in assignment\n");
                errorOccurred = 1;
                return -1;
            }
            else
                return l_expr;
                                            }

            if((l_expr == IVEC2 || l_expr == IVEC3 || l_expr == IVEC4) && (r_expr == INT))
                return INT;
                                                                                                            
            if((l_expr == BVEC2 || l_expr == BVEC3 || l_expr == BVEC4) && (r_expr == BOOL))
                return BOOL;

            if((l_expr == VEC2 || l_expr == VEC3 || l_expr == VEC4) && (r_expr == FLOAT))
                return FLOAT;
            
            break;

        case 6: 
            l_expr = semantic_check(ast->const_declaration.type);
            r_expr = semantic_check(ast->const_declaration.expression);

            if(r_expr == -1 || l_expr == -1)
                return -1;

            if(scope_check(ast->const_declaration.id, ct_scope) == -1) {
                fprintf(errorFile, "Error: Variable cannot be re-declared within the same scope\n");
                errorOccurred = 1;
                return -1;                                                            
            }

            if(l_expr != r_expr){
                fprintf(errorFile, "Error: Type mismatch in assignment\n"
                errorOccurred = 1;
                return -1;
            }
            else
                return l_expr;
                
            type_class = get_tClass(ast->const_declaration.value->id_variable.id);
            if(ast->const_declaration.value->kind != NINT_EXPR && ast->const_declaration.value->kind != NFLOAT_EXPR && ast->const_declaration.value->kind != NBOOL_EXPR && type_class != CONST && type_class != UNIFORM ) {
                fprintf("Error: const qualified variables must be initialized with a literal value or with a uniform variable\n");
                errorOccurred = 1;
                return -1;
            }

            if((l_expr == IVEC2 || l_expr == IVEC3 || l_expr == IVEC4) && (r_expr == INT))
                return INT;

            if((l_expr == VEC2 || l_expr == VEC3 || l_expr == VEC4) && (r_expr == FLOAT))
                return FLOAT;
                                                                                                            
            if((l_expr == BVEC2 || l_expr == BVEC3 || l_expr == BVEC4) && (r_expr == BOOL))
                return BOOL;
            
            break;
        
        case 7:
            
            break;

        case 8:
            l_expr = semantic_check(ast->if_statement.condition);
                        
            if(l_expr != BOOL) {
                fprintf(errorFile, "Error: Condition must be of type to bool");
                errorOccurred = 1;
                return -1;
            }                                     
                 
            else
                semantic_check(ast->if_statement.statement);

            break;

        case 9:
            l_expr = semantic_check(ast->if_else_statement.condition);

            if(l_expr != BOOL) {
                fprintf(errorFile, "Error: Condition must be of type bool\n");
                errorOccurred = 1;                                                        
                return -1;
            }
            
            else {
                semantic_check(ast->if_else_statement.statement);
                semantic_check(ast->if_else_statement.else_statement);
            }
            break;

        case 10:
            ct_scope++;
            r_expr = semantic_check(ast->scope_statement.scope);
            ct_scope--;
            return r_expr;
            break;

        case 11:
            r_expr = semantic_check(ast->unary_expr.right);

            if(ast->unary_expr.op == MINUS) {
                                                    
                if(r_expr == BOOL || r_expr == BVEC2|| r_expr == BVEC3|| rigddht_expr == BVEC4){
                    fprintf(errorFile, "Error: All operands to arithmetic operators must have arithmetic types.\n");
                    errorOccurred = 1;                                                                                                
                    return -1;
                }
                else
                    return r_expr;
            }
            else if(ast->unary_expr.op == NOT) {
                
                if(r_expr != BOOL || r_expr != BVEC2 || r_expr != BVEC3 || r_expr != BVEC4) {
                    fprintf(errorFile, "Error: All operands to logical operators must have boolean types\n");
                    errorOccurred = 1;
                    return -1;
                    }
                else
                    return r_expr;
            }
            else
                return -1;

            break;

        case 12:

            break;

        case 25:
            return ast->type.type_kind;
            break;

            return 0; // failed checks
    }
