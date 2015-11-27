#include "semantic.h"

int ct_scope = 0;
int l_expr, r_expr;
int type, type_class;
int i;

int semantic_check( node *ast) {

    assert(ast);
	int btype;

    switch((int)ast->kind) {
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
            if(scope_check(sym_table, ast->type_declaration.id, ct_scope) == -1) {
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

            if(scope_check(sym_table, ast->assign_declaration.id, ct_scope) == -1) {
                fprintf(errorFile, "Error: Variable cannot be re-declared within the same scope\n");
                errorOccurred = 1;
                return -1;                                                            
            }

            if(l_expr == r_expr)
                return l_expr;

            else {
                fprintf(errorFile, "Error: Type mismatch in assignment\n");
                errorOccurred = 1;
                return -1;
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

            if(scope_check(sym_table, ast->const_declaration.id, ct_scope) == -1) {
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
                
            type_class = get_tClass(sym_table, ast->const_declaration.type->id_variable.id);
            if(ast->const_declaration.type->kind != NINT_EXPR && ast->const_declaration.type->kind != NFLOAT_EXPR && ast->const_declaration.type->kind != NBOOL_EXPR && type_class != _CONST && type_class != UNIFORM ) {
                fprintf(errorFile, "Error: const qualified variables must be initialized with a literal value or with a uniform variable\n");
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
        
		//NASSIGN_STATEMENT
        case 7:
			l_expr = semantic_check(ast->assign_statement.variable);
			r_expr = semantic_check(ast->assign_statement.expression);

			if(l_expr ==  -1 || r_expr == -1){
				return -1;
			}else
				return CheckTypes(r_expr, l_expr);
			
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
            r_expr = semantic_check(ast->prog_scope.scope);
            ct_scope--;
            return r_expr;
            break;
	
		//unary expression
        case 11:
            r_expr = semantic_check(ast->unary_expr.right);
			if(r_expr == -1){
				return -1;
			}
            if(ast->unary_expr.op == MINUS_OPS) {
                                                    
                if(r_expr == BOOL || r_expr == BVEC2|| r_expr == BVEC3|| r_expr == BVEC4){
                    fprintf(errorFile, "Error: All operands to arithmetic operators must have arithmetic types.\n");
                    errorOccurred = 1;                                                                                                
                    return -1;
                }
                else{
					ast->unary_expr.type = r_expr;
                    return r_expr;
				}
            }
            else if(ast->unary_expr.op == NOT_OPS) {
                
                if(r_expr != BOOL || r_expr != BVEC2 || r_expr != BVEC3 || r_expr != BVEC4) {
                    fprintf(errorFile, "Error: All operands to logical operators must have boolean types\n");
                    errorOccurred = 1;
                    return -1;
                    }
                else{
					ast->unary_expr.type = r_expr;
                    return r_expr;
				}
            }
            else
                return -1;
            break;
	
		//NBINARY_EXPR
        case 12:
			r_expr = semantic_check(ast->binary_expr.right);
			l_expr = semantic_check(ast->binary_expr.left);
			btype = CheckTypes(r_expr, l_expr);
			
			if(r_expr == -1 ||l_expr == -1){
				return -1;
			}else if(btype == -1){
					fprintf(errorFile, "Error: The operands to binary operators must have same base types\n");
                    errorOccurred = 1;
                    return -1;
			}else if(vectorChecking(r_expr) == 1 && vectorChecking(l_expr) == 1){
				if(vectorCompare(r_expr, l_expr) == 0){
					fprintf(errorFile, "Error: The vector operands to binary operators must have same order\n");
                    errorOccurred = 1;
                    return -1;
				}else{
					ast->binary_expr.type = btype;
					return btype;
				}
			}else if(ast->binary_expr.op == AND_OPS || ast->binary_expr.op == OR_OPS ||ast->binary_expr.op == EQ_OPS || ast->binary_expr.op == NEQ_OPS){
				if(r_expr != BOOL || r_expr != BVEC2 || r_expr != BVEC3 || r_expr != BVEC4 || l_expr != BOOL || l_expr != BVEC2 || l_expr != BVEC3 || l_expr != BVEC4){
					fprintf(errorFile, "Error: All operands to logical operators must have boolean types\n");
                    errorOccurred = 1;
                    return -1;
				}else if(vectorChecking(r_expr) == 1 || vectorChecking(l_expr) == 1){
					fprintf(errorFile, "Error: The operands to logical operators must be both vectors or both scalars\n");
                    errorOccurred = 1;
                    return -1;
				}else{
					ast->binary_expr.type = btype;
					return btype;
				}
			}else if(ast->binary_expr.op == LESS_OPS || ast->binary_expr.op == LEQ_OPS ||ast->binary_expr.op == GTR_OPS || ast->binary_expr.op == GEQ_OPS || ast->binary_expr.op == PLUS_OPS || ast->binary_expr.op == MINUS_OPS  || ast->binary_expr.op == DIVIDE_OPS || ast->binary_expr.op == POW_OPS || ast->binary_expr.op == TIMES_OPS){
				if(r_expr == BOOL || r_expr == BVEC2|| r_expr == BVEC3|| r_expr == BVEC4 || l_expr == BOOL || l_expr == BVEC2|| l_expr == BVEC3|| l_expr == BVEC4){
					fprintf(errorFile, "Error: All operands to arithmetic operators must have arithmetic types.\n");
                    errorOccurred = 1;                                                                                                
                    return -1;
				}else if( ast->binary_expr.op != TIMES_OPS && (vectorChecking(r_expr) == 1 || vectorChecking(l_expr) == 1)){
					fprintf(errorFile, "Error: The operands to arithmetic operators (except for times) must be both vectors or both scalars\n");
                    errorOccurred = 1;
                    return -1;
				}else{
					ast->binary_expr.type = btype;
					return btype;
				}
			}
			else
				return -1;
		

            break;

        case 13:
            return semantic_check(ast->brackets_expr.expression);
            break;

        case 14:
            type = semantic_check(ast->func_expr.arguments_opt);

            switch(ast->func_expr.func) {
        
                case 0:
                    if(type == IVEC3 || type == IVEC4)
                        return INT;
                    else if(type == VEC3 || type == VEC4)
                        return FLOAT;
                    else
                        return -1;
                    break;

                case 1:
                    if(type == VEC4)
                        return VEC4;
                    else
                        return -1;
                    break;

                case 2:
                    if(type == FLOAT || type == INT)
                        return FLOAT;
                    else
                        return -1;
                    break;
                
                default:
                    fprintf(errorFile, "Error: Fuction argument doesn't match as expected.\n");
                    errorOccurred = 1;
                    return -1;
            }
            break;

        case 15:
            l_expr = semantic_check(ast->type_expr.type);
            r_expr = semantic_check(ast->type_expr.arguments_opt);

            if(l_expr == -1 || r_expr == -1)
                return -1;

            if((l_expr == IVEC2 || l_expr == IVEC3 || l_expr == IVEC4) && (r_expr == INT))
                return INT;

            if((l_expr == VEC2 || l_expr == VEC3 || l_expr == VEC4) && (r_expr == FLOAT))
                return FLOAT;

            if((l_expr == BVEC2 || l_expr == BVEC3 || l_expr == BVEC4) && (r_expr == BOOL))
                return BOOL;

            if(l_expr == r_expr)
                return l_expr;
            else {
                fprintf(errorFile, "Error: Type mismatch found\n");
                errorOccurred = 1;
                return -1;
            }
            break;

        case 16:
            break;
            
        case 17:
            return INT;
            break;

        case 18:
            return FLOAT;
            break;

        case 19:
            return BOOL;
            break;

        case 20:
            break;

        case 21:
            i = ast->array_variable.index;
            type = get_data_type(sym_table, ast->array_variable.id);

            switch(type) {
                    
                case IVEC2:
                    if(i >= 2) {
                        fprintf(errorFile, "Error: i limit exceeded.\n");
                        errorOccurred = 1;                        
                        return -1;
                    }                                                                                                                                                              
                    break;
                case IVEC3:
                    if(i >= 3) {
                        fprintf(errorFile, "Error: i limit exceeded.\n");
                        errorOccurred = 1;
                        return -1;
                    }
                    break;
                case IVEC4:
                    if(i >= 4) {
                        fprintf(errorFile, "Error: i limit exceeded.\n");
                        errorOccurred = 1;
                        return -1;
                    }
                    break;
                case VEC2:
                    if(i >= 2) {
                        fprintf(errorFile, "Error: i limit exceeded.\n");
                        errorOccurred = 1;
                        return -1;
                    }
                    break;
                case VEC3:
                    if(i >= 3) {
                        fprintf(errorFile, "Error: i limit exceeded.\n");
                        errorOccurred = 1;
                        return -1;
                    }
                    break;
                case VEC4:
                    if(i >= 4) {
                        fprintf(errorFile, "Error: i limit exceeded.\n");
                        errorOccurred = 1;
                        return -1;
                    }
                case BVEC2:
                    break;
                    if(i >= 2) {
                        fprintf(errorFile, "Error: i limit exceeded.\n");
                        errorOccurred = 1;
                        return -1;
                    }
                    break;
                case BVEC3:
                    if(i >= 3) {
                        fprintf(errorFile, "Error: i limit exceeded.\n");
                        errorOccurred = 1;
                        return -1;
                    }
                    break;
                case BVEC4:
                    if(i >= 4) {
                        fprintf(errorFile, "Error: i limit exceeded.\n");
                        errorOccurred = 1;
                        return -1;
                    }
                    break;
                default:
                    fprintf(errorFile, "Error: VEC type not found.\n");
                    errorOccurred = 1;
                    return -1;
            }
            break;

        case 22:
            r_expr = semantic_check(ast->args_arguments.arguments);
            l_expr = semantic_check(ast->args_arguments.expression);
                        
            if(l_expr == -1 || r_expr == -1)
                return -1;

            else if(l_expr == r_expr)
                return r_expr;
                                                                                
            else {
                fprintf(errorFile, "Error: Mismatch in arguments\n");
                errorOccurred = 1;
                return -1;
            }
            break;

        case 23:
            return semantic_check(ast->expr_arguments.expression);
            break;

        case 24:
            return semantic_check(ast->arguments_opt.arguments);
            break;

        case 25:
            return ast->type.type_kind;
            break;

        default:
            return -1;
            break;
    }
    return 0; // failed checks
}

//returns 1 if it is vector
int vectorChecking(int type){
	if(type ==  IVEC2 || type == IVEC3 || type == IVEC4 || type == BVEC2 || type == BVEC3 || type == BVEC4 ||type == VEC2 ||type == VEC3 || type == VEC4){
		return 1;

	}else{
		return 0;
	}

}

//return 1 if matches, 0 if mismatch
int vectorCompare(int type1, int type2){
	int isTrue = 0;
	if(type1 == IVEC2 && type2 == IVEC2){
		isTrue = 1;
	}else if(type1 == BVEC2 && type2 == BVEC2){
		isTrue = 1;
	}else if(type1 == VEC2 && type2 == VEC2){
		isTrue = 1;
	}else if(type1 == IVEC3 && type2 == IVEC3){
		isTrue = 1;
	}else if(type1 == BVEC3 && type2 == BVEC3){
		isTrue = 1;
	}else if(type1 == VEC3 && type2 == VEC3){
		isTrue = 1;
	}else if(type1 == IVEC4 && type2 == IVEC4){
		isTrue = 1;
	}else if(type1 == BVEC4 && type2 == BVEC4){
		isTrue = 1;
	}else if(type1 == VEC4 && type2 == VEC4){
		isTrue = 1;
	}else {
		isTrue = 0;
	}
	return isTrue;

}


int CheckTypes(int type1, int type2){
	int isTrue = -1;
	int bt1;
	int bt2;
	
	if(type1 == INT || type1 == IVEC2 || type1 == IVEC3 || type1 == IVEC4){
		bt1 = INT;
	}else if(type1 == BOOL || type1 == BVEC2 || type1 == BVEC3 || type1 == BVEC4){
		bt1 = BOOL;
	}else if(type1 == FLOAT || type1 == VEC2 || type1 == VEC3 || type1 == VEC4){
		bt1 = FLOAT;
	}

	if(type2 == INT || type2 == IVEC2 || type2 == IVEC3 || type2 == IVEC4){
		bt2 = INT;
	}else if(type2 == BOOL || type2 == BVEC2 || type2 == BVEC3 || type2 == BVEC4){
		bt2 = BOOL;
	}else if(type2 == FLOAT || type2 == VEC2 || type2 == VEC3 || type2 == VEC4){
		bt2 = FLOAT;
	}
	
	if(bt1 == INT && bt2 == INT){
		isTrue = INT;
	}else if(bt1 == BOOL && bt2 == BOOL){
		isTrue = BOOL;
	}else if(bt1 == FLOAT && bt2 == FLOAT){
		isTrue = FLOAT;
	}else{
		isTrue = -1;
	}
	
	return isTrue;

}
