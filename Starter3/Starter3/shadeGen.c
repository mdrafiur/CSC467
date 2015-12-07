#include "shadeGen.h"


int InIf = 0;
int IfCompare = 0;
//for statement & declaration
int registerCount = 0;
//for expression
int tempregCount = 0;
//at most 20 nested if_statement;
int cond = 0;
int inCond = 0;
//int scopeNum = 0;
FILE * fragFile;
int Maxtemp = 0;
int Maxcon = 0;

//int boots(node *ast);
//int printToFile(node *ast);

char regComp(int index)  
{
	switch (index) {
	case 0:
		return 'x';
	case 1:
		return 'y';
	case 2:
		return 'z';
	case 3:
		return 'w';
	}
}

int boots(node *ast){
	fragFile = fopen ("frag.txt","w");
	printf("!!ARBfp1.0\n");
	int tmp = printToFile(ast);
	printf("END\n"); 
	return tmp;
}


int printToFile(node *ast){

	if(ast == NULL){
		return 0;
	}
	
	int kind = ast->kind;
	int rightExpr;
	int leftExpr;
	int type;
	
	switch (kind){

	case NSCOPE:
		rightExpr = printToFile(ast->scope.declarations);
		leftExpr = printToFile(ast->scope.statements);
		
		if(rightExpr == -1|| leftExpr == -1){
			return -1;
		}else{
			return 0;
		}
		break;
	case NDECLARATIONS:
		rightExpr = printToFile(ast->declarations.declarations);
    		leftExpr  = printToFile(ast->declarations.declaration); 
		
		if(rightExpr == -1|| leftExpr == -1){
			return -1;
		}else{
			return 0;
		}
		break;
	case NSTATEMENTS:
		rightExpr = printToFile(ast->statements.statements);
		leftExpr  = printToFile(ast->statements.statement);
		if(rightExpr == -1|| leftExpr == -1){
			return -1;
		}else{
			return 0;
		}
		break;


	/*4*/
	case NTYPE_DECLARATION:
		fprintf(fragFile,"TEMP %s;\n", ast->type_declaration.id);
		return 0;
		break;


	case NASSIGN_DECLARATION:
		fprintf(fragFile,"TEMP %s;\n", ast->assign_declaration.id);
		rightExpr = printToFile(ast->assign_declaration.expression);
		if(rightExpr == -1){
			return -1;
		}
		fprintf(fragFile, "MOV %s, temReg%d;\n", ast->assign_declaration.id, tempregCount);
		tempregCount--;
		return 0;
		break;


	case NCONST_DECLARATION:
		fprintf(fragFile,"PARAM %s;\n", ast->const_declaration.id);
		rightExpr = printToFile(ast->const_declaration.expression);
		if(rightExpr == -1){
			return -1;
		}
		fprintf(fragFile, "MOV %s, temReg%d;\n", ast->const_declaration.id, tempregCount);
		tempregCount--;
		return 0;
		break;
	
	case NASSIGN_STATEMENT:
		
		rightExpr = printToFile(ast->assign_statement.expression);
		if(rightExpr == -1){
			return -1;
		}
		
		leftExpr = printToFile(ast->assign_statement.variable);
		
		if(inCond > 0){
			fprintf(fragFile,"CMP ");
			leftExpr = printToFile(ast->assign_statement.variable);
			if(leftExpr == -1){
				return -1;
			}
			fprintf(fragFile,", con%d, ", cond);
			leftExpr = printToFile(ast->assign_statement.variable);
			fprintf(fragFile,", temReg%d;\n", tempregCount);
			tempregCount--;

		}else{
		//fprintf(fragFile,"TEMP %s;\n", ast->assign_statement.variable);
			fprintf(fragFile, "MOV ");
			leftExpr = printToFile(ast->assign_statement.variable);
			fprintf(fragFile, "temReg%d;\n", tempregCount);
			tempregCount--;
		}
		return 0;
		break;

	/*8*/
	case NIF_STATEMENT:
		
		
		cond++;
		if(cond > Maxcon){
			Maxcon++;
			fprintf(fragFile,"TEMP  con%d;\n", cond);
		}
		//if(inCond > 0){
			//fprintf(fragFile,"MOV  con%d, -1;\n", cond);
			
		//}else{
			rightExpr = printToFile(ast->if_statement.condition);
			if(rightExpr == -1){
				return -1;
			}
		//}

		inCond++;
		fprintf(fragFile,"MOV  con%d, temReg%d;\n", cond, tempregCount);
		tempregCount--;
		
		fprintf(fragFile,"#then\n");
		leftExpr  = printToFile(ast->if_statement.statement);
		cond--;
		inCond--;
		if(leftExpr == -1){
			return -1;
		}else{
			return 0;
		}
		break;

		/*9*/
	case NIF_ELSE_STATEMENT:
		
		cond++;
		//if(inCond > 0){
			//fprintf(fragFile,"MOV  con%d, -1;\n", cond);
		//}
		if(cond > Maxcon){
			Maxcon++;
			fprintf(fragFile,"TEMP  con%d;\n", cond);
		}
		rightExpr = printToFile(ast->if_statement.condition);
		if(rightExpr == -1){
			return -1;
		}
		
		inCond++;
		fprintf(fragFile,"MOV  con%d, temReg%d;\n", cond, tempregCount);
		tempregCount--;
		
		fprintf(fragFile,"#then\n");
		leftExpr  = printToFile(ast->if_statement.statement);
		if(leftExpr == -1){
			return -1;
		}
		fprintf(fragFile,"#else\n");
		//if(inCond <= 1){
			//fprintf(fragFile,"MUL con%d, MUL con%d,-1;\n", inCond, inCond);
		//}
		
		leftExpr  = printToFile(ast->if_else_statement.else_statement);
		inCond--;
		cond--;
		if(leftExpr == -1){
			return -1;
		}else{
			return 0;
		}	

		break;

		/*10*/
		case NUNARY_EXPR:
			rightExpr = printToFile(ast->unary_expr.right);
			if(rightExpr == -1){
				return -1;
			}
			fprintf(fragFile,"MUL temReg%d, temReg%d, -1;\n",tempregCount, tempregCount);
			return 0;
			break;
		/*11 This part is not complete!!!*/
		case NBINARY_EXPR:
			rightExpr = printToFile(ast->binary_expr.right);
			if(rightExpr == -1){
				return -1;
			}
			leftExpr = printToFile(ast->binary_expr.left);
			if(leftExpr == -1){
				return -1;
			}
			if(ast->binary_expr.op == AND_OPS){
				fprintf(fragFile,"MIN temReg%d, temReg%d, temReg%d;\n", tempregCount - 1, tempregCount, tempregCount - 1);
				tempregCount --;
			}else if(ast->binary_expr.op == OR_OPS){
				fprintf(fragFile,"MAX temReg%d, temReg%d, temReg%d;\n", tempregCount - 1, tempregCount, tempregCount - 1);
				tempregCount --;
			}else if(ast->binary_expr.op == EQ_OPS){
				fprintf(fragFile,"SUB temReg%d, temReg%d, temReg%d;\n", tempregCount - 1, tempregCount, tempregCount - 1);
				fprintf(fragFile,"SGE temReg%d, temReg%d, 0;\n", tempregCount, tempregCount - 1);
				fprintf(fragFile,"SLT temReg%d, temReg%d, 0;\n", tempregCount - 1, tempregCount - 1);
				tempregCount --;
				fprintf(fragFile,"SUB temReg%d, temReg%d, 1;\n", tempregCount, tempregCount);
				fprintf(fragFile,"CMP temReg%d, temReg%d, -1, 1;\n", tempregCount, tempregCount);
			}else if(ast->binary_expr.op == NEQ_OPS){
				fprintf(fragFile,"SUB temReg%d, temReg%d, temReg%d;\n", tempregCount - 1, tempregCount, tempregCount - 1);
		                fprintf(fragFile,"SGE temReg%d, temReg%d, 0;\n", tempregCount, tempregCount - 1);
		                fprintf(fragFile,"SLT temReg%d, temReg%d, 0;\n", tempregCount - 1, tempregCount - 1);
		                tempregCount--;
		                fprintf(fragFile,"SUB temReg%d, temReg%d, 1;\n", tempregCount, tempregCount);
		                fprintf(fragFile,"CMP temReg%d, temReg%d, 1, -1;\n", tempregCount, tempregCount);
			}else if(ast->binary_expr.op == GEQ_OPS){
		                fprintf(fragFile,"SGE temReg%d, temReg%d, temReg%d;\n", tempregCount - 1, tempregCount, tempregCount - 1);
		                tempregCount--;
		                fprintf(fragFile,"SUB temReg%d, temReg%d, 1;\n", tempregCount, tempregCount);
		                fprintf(fragFile,"CMP temReg%d, temReg%d, -1, 1;\n", tempregCount, tempregCount);
		        }else if(ast->binary_expr.op == LEQ_OPS){
		                fprintf(fragFile,"SGE temReg%d, temReg%d, temReg%d;\n", tempregCount - 1, tempregCount, tempregCount - 1);
		                tempregCount--;
		                fprintf(fragFile,"SUB temReg%d, temReg%d, 1;\n", tempregCount, tempregCount);
		                fprintf(fragFile,"CMP temReg%d, temReg%d, -1, 1;\n", tempregCount, tempregCount);
		        }else if(ast->binary_expr.op == PLUS_OPS){
		                fprintf(fragFile,"ADD temReg%d, temReg%d, temReg%d;\n", tempregCount - 1, tempregCount, tempregCount - 1);
				tempregCount--;
		        }else if(ast->binary_expr.op == MINUS_OPS){
		                fprintf(fragFile,"SUB temReg%d, temReg%d, temReg%d;\n", tempregCount - 1, tempregCount, tempregCount - 1);
				tempregCount--;
		        }else if(ast->binary_expr.op == TIMES_OPS){
		                fprintf(fragFile,"MUL temReg%d, temReg%d, temReg%d;\n", tempregCount - 1, tempregCount, tempregCount - 1);
				tempregCount--;
		        }else if(ast->binary_expr.op == LESS_OPS){
		                fprintf(fragFile,"SUB temReg%d, temReg%d, temReg%d;\n", tempregCount - 1, tempregCount, tempregCount - 1);
		                tempregCount--;
		                fprintf(fragFile,"CMP temReg%d, temReg%d, 1, -1;\n", tempregCount - 1, tempregCount -1);
		        }else if(ast->binary_expr.op == POW_OPS){
		            	fprintf(fragFile,"POW temReg%d, temReg%d, temReg%d;\n", tempregCount - 1, tempregCount, tempregCount - 1);
				tempregCount--;
		        }
            		break;

		case NBRACKETS_EXPR:
			rightExpr = printToFile(ast->brackets_expr.expression);
			return rightExpr;
			break;
		
		case NFUNC_EXPR:
			
			tempregCount++;
			if(tempregCount > Maxtemp){
				fprintf(fragFile,"TEMP temReg%d;\n", tempregCount);
				Maxtemp++;
			}

			if(ast->func_expr.func == 0){
				fprintf(fragFile,"DP3 temReg%d, ", tempregCount);
			}else if(ast->func_expr.func == 1){
				fprintf(fragFile,"LIT temReg%d, ", tempregCount);
			}else if(ast->func_expr.func == 2){
				fprintf(fragFile,"RSQ temReg%d, ", tempregCount);
			}else{
				return -1;
			}
			rightExpr = printToFile(ast->func_expr.arguments_opt);
			return rightExpr;
			break;
			
		case NTYPE_EXPR:
			//leftExpr = printToFile(ast->type_expr.type);
			fprintf(fragFile, "{");
			rightExpr = printToFile(ast->type_expr.arguments_opt);
			fprintf(fragFile, "}");
			return 0;
			break;
			
		case NVAR_EXPR:
			rightExpr = printToFile(ast->var_expr.variable);
			return rightExpr;
			break;
			
		case NINT_EXPR:
			tempregCount++;
			if(tempregCount > Maxtemp){
				fprintf(fragFile,"TEMP temReg%d;\n", tempregCount);
				Maxtemp++;
			}
			fprintf(fragFile, "MOV temReg%d, %d.0;\n", tempregCount, ast->int_expr.number);
			return 0;
			break;
			
		case NFLOAT_EXPR:
			tempregCount++;
			if(tempregCount > Maxtemp){
				fprintf(fragFile,"TEMP temReg%d;\n", tempregCount);
				Maxtemp++;
			}
			fprintf(fragFile, "MOV temReg%d, %f;\n", tempregCount, ast->float_expr.number);
			return 0;
			break;
			
		case NBOOL_EXPR:
			tempregCount++;
			if(tempregCount > Maxtemp){
				fprintf(fragFile,"TEMP temReg%d;\n", tempregCount);
				Maxtemp++;
			}
			if(ast->bool_expr.boolean == 0){
				fprintf(fragFile,"MOV  temReg%d, 1 ;\n", tempregCount);
			}else if(ast->bool_expr.boolean == 1){
				fprintf(fragFile,"MOV  temReg%d, -1 ;\n", tempregCount);
			}else{
				return -1;
			}
			return 0;
			break;
			
		case NID_VARIABLE:
			//char *name = ast->id_variable.id;
		
			if (strcmp(ast->id_variable.id, "gl_FragColor") == 0) {
				fprintf(fragFile, "result.color");
				return 0;
			} 
			else if (strcmp(ast->id_variable.id, "gl_FragDepth") == 0) {
				fprintf(fragFile, "result.depth");
				return 0;
			} 
			else if (strcmp(ast->id_variable.id, "gl_FragCoord") == 0) {
				fprintf(fragFile, "fragment.position");
				return 0;
			} 
			else if (strcmp(ast->id_variable.id, "gl_TexCoord") == 0) {
				fprintf(fragFile, "fragment.texcoord");
				return 0;
			} 
			else if (strcmp(ast->id_variable.id, "gl_Color") == 0) {
				fprintf(fragFile, "fragment.color");
				return 0;
			} 
			else if (strcmp(ast->id_variable.id, "gl_Secondary") == 0) {
				fprintf(fragFile, "fragment.color.secondary");
				return 0;
			} 
			else if (strcmp(ast->id_variable.id, "gl_FogFragCoord") == 0) {
				fprintf(fragFile, "fragment.fogcoord");
				return 0;
			} 
			else if (strcmp(ast->id_variable.id, "gl_Light_Half") == 0) {
				fprintf(fragFile, "state.light[0].half");
				return 0;
			} 
			else if (strcmp(ast->id_variable.id, "gl_Light_Ambient") == 0) {
				fprintf(fragFile, "state.lightmodel.ambient");
				return 0;
			} 
			else if (strcmp(ast->id_variable.id, "gl_Material_Shininess") == 0) {
				fprintf(fragFile, "state.material.shininess");
				return 0;
			} 
			else if (strcmp(ast->id_variable.id, "env1") == 0) {
				fprintf(fragFile, "program.env[1]");
				return 0;
			} 
			else if (strcmp(ast->id_variable.id, "env2") == 0) {
				fprintf(fragFile, "program.env[2]");
				return 0;
			} 
			else if (strcmp(ast->id_variable.id, "env3") == 0) {
				fprintf(fragFile, "program.env[3]");
				return 0;
			} 
			else {
				fprintf(fragFile, "%s", ast->id_variable.id);
				return 0;
			}
			return 0;
			break;
			
		case NARRAY_VARIABLE:
			//char *id = ast->array_variable.id;
			type = get_data_type(sym_table, ast->array_variable.id);
			//int index = ast->array_variable.index;

			if (strcmp(ast->array_variable.id, "gl_FragColor") == 0) {
				fprintf(fragFile, "result.color.%c", regComp(ast->array_variable.index));
				return 0;
		
			} else if (strcmp(ast->array_variable.id, "gl_FragDepth") == 0) {
				fprintf(fragFile, "result.depth.%c", regComp(ast->array_variable.index));
				return 0;
		
			} else if (strcmp(ast->array_variable.id, "gl_FragCoord") == 0) {
				fprintf(fragFile, "fragment.position.%c", regComp(ast->array_variable.index));
				return 0;
		
			} else if (strcmp(ast->array_variable.id, "gl_TexCoord") == 0) {
				fprintf(fragFile, "fragment.texcoord.%c", regComp(ast->array_variable.index));
				return 0;
		
			} else if (strcmp(ast->array_variable.id, "gl_Color") == 0) {
				fprintf(fragFile, "fragment.color.%c", regComp(ast->array_variable.index));
				return 0;
		
			} else if (strcmp(ast->array_variable.id, "gl_Secondary") == 0) {
				fprintf(fragFile, "fragment.color.secondary.%c", regComp(ast->array_variable.index));
				return 0;
		
			} else if (strcmp(ast->array_variable.id, "gl_FogFragCoord") == 0) {
				fprintf(fragFile, "fragment.fogcoord.%c", regComp(ast->array_variable.index));
				return 0;
		
			} else if (strcmp(ast->array_variable.id, "gl_Light_Half") == 0) {
				fprintf(fragFile, "state.light[0].half.%c", regComp(ast->array_variable.index));
				return 0;
		
			} else if (strcmp(ast->array_variable.id, "gl_Light_Ambient") == 0) {
				fprintf(fragFile, "state.lightmodel.ambient.%c", regComp(ast->array_variable.index));
				return 0;
		
			} else if (strcmp(ast->array_variable.id, "gl_Material_Shininess") == 0) {
				fprintf(fragFile, "state.material.shininess.%c", regComp(ast->array_variable.index));
				return 0;
		
			} else if (strcmp(ast->array_variable.id, "env1") == 0) {
				fprintf(fragFile, "program.env[1].%c", regComp(ast->array_variable.index));
				return 0;
		
			} else if (strcmp(ast->array_variable.id, "env2") == 0) {
				fprintf(fragFile, "program.env[2].%c", regComp(ast->array_variable.index));
				return 0;
		
			} else if (strcmp(ast->array_variable.id, "env3") == 0) {
				fprintf(fragFile, "program.env[3].%c", regComp(ast->array_variable.index));
				return 0;
		
			} else {
				fprintf(fragFile, "%s.%c", ast->array_variable.id, regComp(ast->array_variable.index));
				return 0;
			}
			break;
			
		case NARGS_ARGUMENTS:
			rightExpr = printToFile(ast->args_arguments.arguments);
			leftExpr = printToFile(ast->args_arguments.arguments);
			if(rightExpr == -1|| leftExpr == -1){
				return -1;
			}else{
				return 0;
			}
			break;
			
		case NEXPR_ARGUMENTS:
			rightExpr = printToFile(ast->expr_arguments.expression);
			return rightExpr;
			break;
			
		case NARGUMENTS_OPT:
			rightExpr = printToFile(ast->arguments_opt.arguments);
			return rightExpr;
			break;
			
		case NTYPE:
			break;
			
		case NPROG_SCOPE:
			//scopeNum++;
			rightExpr = printToFile(ast->prog_scope.scope);
			//scopeNum--;
			return rightExpr;
			break;
	}


}
