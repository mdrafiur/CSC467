#include "semantic.h"

int scopeNum = 0;
int check_prog_scope;
int numArgs, x;
int lhs, rhs;
int type, type_class;

int semantic_check(node *ast) {

    if(!ast)
        return 0;

    int btype;

    switch((int)ast->kind) {
        case UNKNOWN:
            break;

        case NSCOPE:
            lhs = semantic_check(ast->scope.declarations);
            if(lhs == -1)
                return -1;

            rhs = semantic_check(ast->scope.statements);
            if(rhs == -1)
                return -1;

            if(lhs > rhs)
                return lhs;
            else
                return rhs;
            break;

        case NDECLARATIONS:
            lhs = semantic_check(ast->declarations.declarations);
            if(lhs == -1)
                return -1;

            rhs = semantic_check(ast->declarations.declaration);
            if(rhs == -1)
                return -1;
            else
                return rhs;
            break;

        case NSTATEMENTS:
            lhs = semantic_check(ast->statements.statements);
            if(lhs == -1)
                return -1;

            rhs = semantic_check(ast->statements.statement);
            if(rhs == -1)
                return -1;
            else
                return rhs;
            break;

        case NTYPE_DECLARATION:
            if(isVarDeclared(sym_table, ast->type_declaration.id, scopeNum)) {
                fprintf(errorFile, "Error: Variable cannot be re-declared within the same scope\n");
                errorOccurred = 1;    
                return -1;
            }
            else
                return semantic_check(ast->type_declaration.type);
            break;

        case NASSIGN_DECLARATION:
            lhs = semantic_check(ast->assign_declaration.type);
            if(lhs == -1)
                return -1;

            rhs = semantic_check(ast->assign_declaration.expression);
            if(rhs == -1)
                return -1;

            if(isVarDeclared(sym_table, ast->assign_declaration.id, scopeNum)) {
                fprintf(errorFile, "Error: Variable cannot be re-declared within the same scope\n");
                errorOccurred = 1;
                return -1;                                                            
            }

            if(lhs == rhs)
                return lhs;

            else if((lhs == IVEC2 || lhs == IVEC3 || lhs == IVEC4) && (rhs == INT))
                return INT;

            else if((lhs == BVEC2 || lhs == BVEC3 || lhs == BVEC4) && (rhs == BOOL))
                return BOOL;

            else if((lhs == VEC2 || lhs == VEC3 || lhs == VEC4) && (rhs == FLOAT))
                return FLOAT;

            else {
                fprintf(errorFile, "Error: Declaration failed, type mismatch in assignment\n");
                errorOccurred = 1;
                return -1;
            }
            break;

        case NCONST_DECLARATION: 
            lhs = semantic_check(ast->const_declaration.type);
            if(lhs == -1)
                return -1;

            rhs = semantic_check(ast->const_declaration.expression);
            if(rhs == -1)
                return -1;

            type_class = get_tClass(sym_table, ast->const_declaration.type->id_variable.id);
            if(ast->const_declaration.type->kind != NINT_EXPR && ast->const_declaration.type->kind != NFLOAT_EXPR && ast->const_declaration.type->kind != NBOOL_EXPR && type_class != _CONST && type_class != UNIFORM) {
                fprintf(errorFile, "Error: 'const' qualified variables must be initialized with a literal value or with a uniform variable\n");
                errorOccurred = 1;
                return -1;
            }

            if(lhs == rhs)
                return lhs;

            else if((lhs == IVEC2 || lhs == IVEC3 || lhs == IVEC4) && (rhs == INT))
                return INT;

            else if((lhs == VEC2 || lhs == VEC3 || lhs == VEC4) && (rhs == FLOAT))
                return FLOAT;

            else if((lhs == BVEC2 || lhs == BVEC3 || lhs == BVEC4) && (rhs == BOOL))
                return BOOL;

            else {
                fprintf(errorFile, "Error: Type mismatch in assignment\n");
                errorOccurred = 1;
                return -1;
            }
            break;

        case NASSIGN_STATEMENT:
            lhs = semantic_check(ast->assign_statement.variable);
            if(lhs == -1)
                return -1;

            rhs = semantic_check(ast->assign_statement.expression);
            if(rhs == -1)
                return -1;
            else
                return CheckTypes(rhs, lhs);

            break;

        case NIF_STATEMENT:
            lhs = semantic_check(ast->if_statement.condition);

            if(lhs == -1)
                return -1;

            if(lhs == BOOL) 
                semantic_check(ast->if_statement.statement);

            else {
                fprintf(errorFile, "Error: Condition must be of type 'bool'\n");
                errorOccurred = 1;
                return -1;
            }                                     
            break;

        case NIF_ELSE_STATEMENT:
            lhs = semantic_check(ast->if_else_statement.condition);
            if(lhs == -1)
                return -1;

            if(lhs == BOOL) {
                semantic_check(ast->if_else_statement.statement);
                semantic_check(ast->if_else_statement.else_statement);
            }
            else {
                fprintf(errorFile, "Error: Condition must be of type 'bool'\n");
                errorOccurred = 1;                                                        
                return -1;
            }
            break;

        case NSCOPE_STATEMENT:
            scopeNum++;
            check_prog_scope = semantic_check(ast->prog_scope.scope);
            scopeNum--;
            return check_prog_scope;
            break;

        //unary expression
        case NUNARY_EXPR:
            rhs = semantic_check(ast->unary_expr.right);
            if(rhs == -1)
                return -1;

            if(ast->unary_expr.op == MINUS_OPS) {

                if(rhs == BOOL || rhs == BVEC2|| rhs == BVEC3|| rhs == BVEC4){
                    fprintf(errorFile, "Error: All operands to arithmetic operators must have arithmetic types\n");
                    errorOccurred = 1;                                                                                                
                    return -1;
                }
                else
                    return rhs;
            }
            if(ast->unary_expr.op == NOT_OPS) {

                if(rhs != BOOL || rhs != BVEC2 || rhs != BVEC3 || rhs != BVEC4) {
                    fprintf(errorFile, "Error: All operands to logical operators must have boolean types\n");
                    errorOccurred = 1;
                    return -1;
                }
                else
                    return rhs;
            }
            break;

        case NBINARY_EXPR:
            rhs = semantic_check(ast->binary_expr.right);
            lhs = semantic_check(ast->binary_expr.left);
            btype = CheckTypes(rhs, lhs);

            if(rhs == -1 || lhs == -1){
                return -1;
            }else if(btype == -1){
                fprintf(errorFile, "Error: The operands to binary operators must have same base types\n");
                errorOccurred = 1;
                return -1;
            }else if(vectorChecking(rhs) == 1 && vectorChecking(lhs) == 1){
                if(vectorCompare(rhs, lhs) == 0){
                    fprintf(errorFile, "Error: The vector operands to binary operators must have same order\n");
                    errorOccurred = 1;
                    return -1;
                }else{
                    btype = BOOL;
                    ast->binary_expr.type = btype;
                    return btype;
                }
            }else if(ast->binary_expr.op == AND_OPS || ast->binary_expr.op == OR_OPS ||ast->binary_expr.op == EQ_OPS || ast->binary_expr.op == NEQ_OPS){
                if(rhs != BOOL || rhs != BVEC2 || rhs != BVEC3 || rhs != BVEC4 || lhs != BOOL || lhs != BVEC2 || lhs != BVEC3 || lhs != BVEC4){
                    fprintf(errorFile, "Error: All operands to logical operators must have boolean types\n");
                    errorOccurred = 1;
                    return -1;
                }else if((vectorChecking(rhs) && !vectorChecking(lhs)) || (!vectorChecking(rhs) && vectorChecking(lhs))){
                    fprintf(errorFile, "Error: The operands to logical operators must be both vectors or both scalars\n");
                    errorOccurred = 1;
                    return -1;
                }else{
                    btype = BOOL;
                    ast->binary_expr.type = btype;
                    return btype;
                }
            }else if(ast->binary_expr.op == LESS_OPS || ast->binary_expr.op == LEQ_OPS ||ast->binary_expr.op == GTR_OPS || ast->binary_expr.op == GEQ_OPS || ast->binary_expr.op == PLUS_OPS || ast->binary_expr.op == MINUS_OPS  || ast->binary_expr.op == DIVIDE_OPS || ast->binary_expr.op == POW_OPS || ast->binary_expr.op == TIMES_OPS){
                if(rhs == BOOL || rhs == BVEC2|| rhs == BVEC3|| rhs == BVEC4 || lhs == BOOL || lhs == BVEC2|| lhs == BVEC3|| lhs == BVEC4){
                    fprintf(errorFile, "Error: All operands to arithmetic operators must have arithmetic types.\n");
                    errorOccurred = 1;                                                                                                
                    return -1;
                }else if( ast->binary_expr.op != TIMES_OPS && (vectorChecking(rhs) == 1 || vectorChecking(lhs) == 1)){
                    fprintf(errorFile, "Error: The operands to arithmetic operators (except for times) must be both vectors or both scalars\n");
                    errorOccurred = 1;
                    return -1;
                }else{
                    btype = BOOL;
                    ast->binary_expr.type = btype;
                    return btype;
                }
            }
            else
                return -1;

            break;

        case NBRACKETS_EXPR:
            return semantic_check(ast->brackets_expr.expression);
            break;

        case NFUNC_EXPR:
            rhs = semantic_check(ast->func_expr.arguments_opt);

            if(rhs == -1)
                return -1;

            switch(ast->func_expr.func) {

                case 0:
                    if(rhs == IVEC3 || rhs == IVEC4)
                        return INT;
                    else if(rhs == VEC3 || rhs == VEC4)
                        return FLOAT;
                    else {
                        fprintf(errorFile, "Error: Function argument doesn't match as expected ('db3' supports arguments of type 'vec3', 'vec4', 'ivec3' & 'ivec4')\n");
                        errorOccurred = 1;
                        return -1;
                    }
                    break;

                case 1:
                    if(rhs == VEC4)
                        return VEC4;
                    else {
                        fprintf(errorFile, "Error: Function argument doesn't match as expected ('lit' only supports argument of type 'vec4')\n");
                        errorOccurred = 1;
                        return -1;
                    }
                    break;

                case 2:
                    if(rhs == FLOAT || rhs == INT)
                        return FLOAT;
                    else {
                        fprintf(errorFile, "Error: Function argument doesn't match as expected ('rsq' supports arguments of type 'int' & 'float')\n");
                        errorOccurred = 1;
                        return -1;
                    }
                    break;

                default:
                    fprintf(errorFile, "Error: Function name doesn't match as expected (supported function names - 'db3', 'lit' & 'rsq')\n");
                    errorOccurred = 1;
                    return -1;
                    break;
            }
            break;

        case NTYPE_EXPR:
            lhs = semantic_check(ast->type_expr.type);
            rhs = semantic_check(ast->type_expr.arguments_opt);

            if(lhs == -1 || rhs == -1)
                return -1;

            numArgs = countArg(ast->type_expr.arguments_opt);

            if((lhs == IVEC2 && numArgs == 2) || (lhs == IVEC3 && numArgs == 3) || (lhs == IVEC4 && numArgs == 4) || (lhs == BVEC2 && numArgs == 2) || (lhs == BVEC3 && numArgs == 3) || (lhs == BVEC4 && numArgs == 4) || (lhs == VEC2 && numArgs == 2) || (lhs == VEC3 && numArgs == 3) || (lhs == VEC4 && numArgs == 4) || (lhs == BOOL && numArgs == 1) || (lhs == INT && numArgs == 1) || (lhs == FLOAT && numArgs == 1))
                ;
            else {
                fprintf(errorFile, "Error: Costructors for basic types (bool, int, float) must have one argument and vector types must have as many arguments as there are elements in the vector\n");
                errorOccurred = 1;
                return -1;
            }

            if(lhs == rhs)
                return lhs;

            else if((lhs == IVEC2 || lhs == IVEC3 || lhs == IVEC4) && (rhs == INT))
                return INT;

            else if((lhs == VEC2 || lhs == VEC3 || lhs == VEC4) && (rhs == FLOAT))
                return FLOAT;

            else if((lhs == BVEC2 || lhs == BVEC3 || lhs == BVEC4) && (rhs == BOOL))
                return BOOL;
            
            else {
                fprintf(errorFile, "Error: Type mismatch found\n");
                errorOccurred = 1;
                return -1;
            }
            break;

        case NVAR_EXPR:
            return semantic_check(ast->var_expr.variable);            
            break;

        case NINT_EXPR:
            return INT;
            break;

        case NFLOAT_EXPR:
            return FLOAT;
            break;

        case NBOOL_EXPR:
            return BOOL;
            break;

        case NID_VARIABLE:
            if(!isVarDeclaredInScope(sym_table, ast->id_variable.id, scopeNum)) {
                fprintf(errorFile, "Error: Variable cannot be used before it is declared\n");
                errorOccurred = 1;
                return -1;
            }
            else {
                type_class = get_tClass(sym_table, ast->id_variable.id); 
                if(type_class == _CONST) {
                    fprintf(errorFile, "Error: Variable cannot be used before it is declared\n");
                    errorOccurred = 1;
                    return -1;
                }
                return get_data_type(sym_table, ast->id_variable.id);
            }
            break;

        case NARRAY_VARIABLE:
            x = ast->array_variable.index;
            type = get_data_type(sym_table, ast->array_variable.id);

            if(type != IVEC2 || type != IVEC3 || type != IVEC4 || type != BVEC2 || type != BVEC3 || type != BVEC4 || type != VEC2 || type != VEC3 || type != VEC4) {
            fprintf(errorFile, "Error: Only 'vec' type supported\n");
                errorOccurred = 1;
                return -1;
            }
            else if((type == IVEC2 && x < 2) || (type == IVEC3 && x < 3) || (type == IVEC4 && x < 4) || (type == BVEC2 && x < 2) || (type == BVEC3 && x < 3) || (type == BVEC4 && x < 4) || (type == VEC2 && x < 2) || (type == VEC3 && x < 3) || (type == VEC4 && x < 4))
                ;
            else {
                fprintf(errorFile, "Error: Index limit exceeded\n");
                errorOccurred = 1;
                return -1;
            }

            if(type == IVEC2 || type == IVEC3 || type == IVEC4)
                return INT;
            
            if(type == BVEC2 || type == BVEC3 || type == BVEC4)
                return BOOL;

            if(type == VEC2 || type == VEC3 || type == VEC4)
                return FLOAT;

            break;

        case NARGS_ARGUMENTS:
            lhs = semantic_check(ast->args_arguments.arguments);
            rhs = semantic_check(ast->args_arguments.expression);

            if(lhs == -1 || rhs == -1)
                return -1;

            else if(lhs == rhs)
                return lhs;

            else {
                fprintf(errorFile, "Error: Mismatch in arguments\n");
                errorOccurred = 1;
                return -1;
            }
            break;

        case NEXPR_ARGUMENTS:
            return semantic_check(ast->expr_arguments.expression);
            break;

        case NARGUMENTS_OPT:
            return semantic_check(ast->arguments_opt.arguments);
            break;

        case NTYPE:
            return ast->type.type_kind;
            break;

        case NPROG_SCOPE:
            scopeNum++;
            check_prog_scope = semantic_check(ast->prog_scope.scope);
            scopeNum--;
            return check_prog_scope;
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

int countArg(node *arg)
{
    int count = 0;

    switch(arg->kind) {
        case NTYPE_EXPR:
            return ++count;
            break;

        case NFUNC_EXPR:
            return ++count;
            break;

        case NBRACKETS_EXPR:
            return ++count;
            break;

        case NBINARY_EXPR:
            return ++count;
            break;

        case NUNARY_EXPR:
            return ++count;
            break;

        case NINT_EXPR:
            return ++count;
            break;

        case NFLOAT_EXPR:
            return ++count;
            break;

        case NBOOL_EXPR:
            return ++count;
            break;

        case NARRAY_VARIABLE:
            return ++count;
            break;

        case NVAR_EXPR:
            return ++count;
            break;

        case NID_VARIABLE:
            return ++count;
            break;

        case NARGS_ARGUMENTS:
            ++count;
            count += countArg(ast->args_arguments.arguments);
            return count;
            break;

        case NEXPR_ARGUMENTS:
            return countArg(ast->expr_arguments.expression);
            break;

        default:
            return -1;
            break;
    }
    return count;
}
