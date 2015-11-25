#include "semantic.h"

int ct_scope = 0;
int right_expr, left_expr;
int type_class;

int semantic_check( node *ast) {

    assert(ast);

    switch((int)ast->kind){
        case 0:
            break;

        case 1:
            right_expr = semantic_check(ast->scope.declarations);
            left_expr = semantic_check(ast->scope.statements);

            if(right_expr == -1 || left_expr == -1)
                return -1;

            return 0;
            break;

        case 2:
            left_expr = semantic_check(ast->declarations.declaration);
            right_expr = semantic_check(ast->declarations.declarations);

            if(right_expr == -1 || left_expr == -1)
                return -1;

            return left_expr;
            break;

        case 3:
            left_expr = semantic_check(ast->statements.statement);
            right_expr = semantic_check(ast->statements.statements);

            if(right_expr == -1 || left_expr == -1)
                return -1;

            return left_expr;
            break;

        case 4:
            if(scope_check(ast->type_declaration.id, ct_scope) == -1) {
                fprintf(errorFile.txt, "Error: Variable cannot be re-declared within the same scope\n");
                errorOccurred = 1;    
                return -1;
            }
            else
                return semantic_check(ast->type_declaration.type);
                                                                        
            break;
            
        case 5:
            return ast->type.type_kind;
            break;

        case 6:
            left_expr = semantic_check(ast->assign_declaration.type);
            right_expr = semantic_check(ast->assign_declaration.expression);

            if(right_expr == -1 || left_expr == -1)
                return -1;

            if(scope_check(ast->assign_declaration.id, ct_scope) == -1) {
                fprintf(errorFile.txt, "Error: Variable cannot be re-declared within the same scope\n");
                errorOccurred = 1;
                return -1;                                                            
            }

            if(left_expr != right_expr){
                fprintf(errorFile.txt, "Error: Type mismatch in assignment\n"
                return -1;
            }
            else
                return left_expr;
                                            }

            if((left_expr == IVEC2 || left_expr == IVEC3 || left_expr == IVEC4) && (right_expr == INT))
                return INT;
                                                                                                            
            if((left_expr == BVEC2 || left_expr == BVEC3 || left_expr == BVEC4) && (right_expr == BOOL))
                return BOOL;

            if((left_expr == VEC2 || left_expr == VEC3 || left_expr == VEC4) && (right_expr == FLOAT))
                return FLOAT;
            
            break;

        case 7: 
            left_expr = semantic_check(ast->const_declaration.type);
            right_expr = semantic_check(ast->const_declaration.expression);

            if(right_expr == -1 || left_expr == -1)
                return -1;

            if(scope_check(ast->const_declaration.id, ct_scope) == -1) {
                fprintf(errorFile.txt, "Error: Variable cannot be re-declared within the same scope\n");
                errorOccurred = 1;
                return -1;                                                            
            }

            if(left_expr != right_expr){
                fprintf(errorFile.txt, "Error: Type mismatch in assignment\n"
                errorOccurred = 1;
                return -1;
            }
            else
                return left_expr;
                
            type_class = get_tClass(ast->const_declaration.value->id_variable.id);
            if(ast->const_declaration.value->kind == NINT_EXPR || ast->const_declaration.value->kind == NFLOAT_EXPR || ast->const_declaration.value->kind == NBOOL_EXPR || type_class == CONST || type_class == UNIFORM ) {
                ;
            else {
                fprintf("Error: const qualified variables must be initialized with a literal value or with a uniform variable\n");
                errorOccurred = 1;
                return -1;
            }

            if((left_expr == IVEC2 || left_expr == IVEC3 || left_expr == IVEC4) && (right_expr == INT))
                return INT;
                                                                                                            
            if((left_expr == BVEC2 || left_expr == BVEC3 || left_expr == BVEC4) && (right_expr == BOOL))
                return BOOL;

            if((left_expr == VEC2 || left_expr == VEC3 || left_expr == VEC4) && (right_expr == FLOAT))
                return FLOAT;
            
            break;
        
        case 8:
            
            break;

        case 9:
            left_expr = semantic_check(ast->if_statement.condition);
                        
            if(left_expr == -1)
                return -1;

            else if(left_expr != BOOL) {
                fprintf(errorFile.txt, "Error: Condition must be of type to bool");
                errorOccurred = 1;
                return -1;
            }                                     
                 
            else
                semantic_check(ast->if_statement.statement);

            break;

        case 10:
            left_expr = semantic_check(ast->if_else_statement.condition);

            if(left_expr == -1)
                return -1;

            else if(left_expr != BOOL) {
                fprintf(errorFile.txt, "Error: Condition must be of type bool\n");
                errorOccurred = 1;                                                        
                return -1;
            }
            
            else {
                semantic_check(ast->if_else_statement.statement);
                semantic_check(ast->if_else_statement.else_statement);
            }
            break;

        case 11:
            ct_scope++;
            right_expr = semantic_check(ast->scope_statement.scope);
            ct_scope--;
            return right_expr;
            break;

        case 12:
            right_expr = semantic_check(ast->unary_expr.right);

            if(right_expr == -1)
                return -1;

            if(ast->unary_expr.op == MINUS) {
                                                    
                if(right_expr == BOOL || right_expr == BVEC2|| right_expr == BVEC3|| rigddht_expr == BVEC4){
                    fprintf(errorFile.txt, "Error: All operands to arithmetic operators must have arithmetic types.\n");
                    errorOccurred = 1;                                                                                                
                    return -1;
                }
                else
                    return right_expr;
            }
            else if(ast->unary_expr.op == NOT) {
                
                if(right_expr != BOOL || right_expr != BVEC2 || right_expr != BVEC3 || right_expr != BVEC4) {
                    fprintf(errorFile.txt "Error: All operands to logical operators must have boolean types\n");
                    errorOccurred = 1;
                    return -1;
                    }
                else
                    return right_expr;
            }                                       
            break;

        case 13:

            break;

            return 0; // failed checks
    }
