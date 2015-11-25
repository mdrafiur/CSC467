#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

#include "ast.h"
#include "common.h"
#include "parser.tab.h"
/*#include "symtable.h"*/

#define DEBUG_PRINT_TREE 0

node *ast = NULL;

node *ast_allocate(node_kind kind, ...) {
  va_list args;

  // make the node
  node *ast = (node *) malloc(sizeof(node));
  memset(ast, 0, sizeof *ast);
  ast->kind = kind;

  va_start(args, kind); 

  switch(kind) {
  
  // ...
	case NSCOPE:
		ast->scope.declarations = va_arg(args, node *);
    	ast->scope.statements = va_arg(args, node *);
		ast->scope.l = va_arg(args, int);
   		break;
  
	case NDECLARATIONS:
		ast->declarations.declarations = va_arg(args, node *);
    	ast->declarations.declaration = va_arg(args, node *);
		ast->declarations.l = va_arg(args, int);
   		break;

	case NSTATEMENTS:
		ast->statements.statements = va_arg(args, node *);
    	ast->statements.statement = va_arg(args, node *);
		ast->statements.l = va_arg(args, int);
   		break;

	/*4*/
	case NTYPE_DECLARATION:
		ast->type_declaration.type = va_arg(args, node *);
    	ast->type_declaration.id = va_arg(args, char *);
		ast->type_declaration.l = va_arg(args, int);
   		break;

	case NASSIGN_DECLARATION:
		ast->assign_declaration.type = va_arg(args, node *);
		ast->assign_declaration.id = va_arg(args, char *);
		ast->assign_declaration.expression = va_arg(args, node *);
		ast->assign_declaration.l = va_arg(args, int);
		break;

	case NCONST_DECLARATION:
		ast->const_declaration.type = va_arg(args, node *);
		ast->const_declaration.id = va_arg(args, char *);
		ast->const_declaration.expression = va_arg(args, node *);
		ast->const_declaration.l = va_arg(args, int);
		break;

	/*7*/
	case NASSIGN_STATEMENT:
		ast->assign_statement.variable = va_arg(args, node *);
		ast->assign_statement.expression = va_arg(args, node *);
		ast->assign_statement.l = va_arg(args, int);
		break;
	
	case NIF_STATEMENT:
		ast->if_statement.condition = va_arg(args, node *);
		ast->if_statement.statement = va_arg(args, node *);
		ast->if_statement.l = va_arg(args, int);
		break;

	case NIF_ELSE_STATEMENT:
		ast->if_else_statement.condition = va_arg(args, node *);
		ast->if_else_statement.statement = va_arg(args, node *);
		ast->if_else_statement.else_statement = va_arg(args, node *);
		ast->scope.l = va_arg(args, int);
		break;

	case NSCOPE_STATEMENT:
		ast->scope_statement.scope = va_arg(args, node *);
		ast->scope_statement.l = va_arg(args, int);
		break;
	
	/*11*/
	case NUNARY_EXPR:
		ast->unary_expr.op = va_arg(args, int);
    	ast->unary_expr.right = va_arg(args, node *);
		ast->scope.l = va_arg(args, int);
    	break;
		
	case NBINARY_EXPR:
		ast->binary_expr.op = va_arg(args, int);
    	ast->binary_expr.left = va_arg(args, node *);
    	ast->binary_expr.right = va_arg(args, node *);
		ast->binary_expr.l = va_arg(args, int);
    	break;

	case NBRACKETS_EXPR:
		ast->brackets_expr.expression = va_arg(args, node *);
		ast->brackets_expr.l = va_arg(args, int);
		break;

	case NFUNC_EXPR:
		ast->func_expr.func = va_arg(args, int);
		ast->func_expr.arguments_opt = va_arg(args, node *);
		ast->func_expr.l = va_arg(args, int);
		break;
	
	case NTYPE_EXPR:
		ast->type_expr.type = va_arg(args, node *);
		ast->type_expr.arguments_opt = va_arg(args, node *);
		ast->type_expr.l = va_arg(args, int);
		break;

	case NVAR_EXPR:
		ast->var_expr.variable = va_arg(args, node *);
		ast->var_expr.l = va_arg(args, int);
		break;

	case NINT_EXPR:
		ast->int_expr.number = va_arg(args, int);
		ast->scope.l = va_arg(args, int);
		break;

	case NFLOAT_EXPR:
		ast->float_expr.number = va_arg(args, double);
		ast->float_expr.l = va_arg(args, int);
		break;

	case NBOOL_EXPR:
		ast->bool_expr.boolean = va_arg(args, int);
		ast->bool_expr.l = va_arg(args, int);
		break;
	
	/*20*/
	case NID_VARIABLE:
		ast->id_variable.id = va_arg(args, char *);
		ast->id_variable.l = va_arg(args, int);
		break;

	case NARRAY_VARIABLE:
		ast->array_variable.id = va_arg(args, char *);
		ast->array_variable.index = va_arg(args, int);
		ast->array_variable.l = va_arg(args, int);
		break;

	case NARGS_ARGUMENTS:
		ast->args_arguments.arguments = va_arg(args, node *);
		ast->args_arguments.expression = va_arg(args, node *);
		ast->args_arguments.l = va_arg(args, int);
		break;
	
	case NEXPR_ARGUMENTS:
		ast->expr_arguments.expression = va_arg(args, node *);
		ast->expr_arguments.l = va_arg(args, int);
		break;

	case NARGUMENTS_OPT:
		ast->arguments_opt.arguments = va_arg(args, node *);
		ast->arguments_opt.l = va_arg(args, int);
		break;
	
	case NTYPE:
		ast->type.type_kind = va_arg(args, int);
		ast->type.l = va_arg(args, int);
		break;

  default: break;
  }

  va_end(args);

  return ast;
}

void ast_free(node *ast) {
	int kind = ast->kind;
	if(ast == NULL)
		return;
	
	switch(kind){

		case NSCOPE:
			ast_free(ast->scope.declarations);
			ast_free(ast->scope.statements);
			free(ast);
			ast = NULL;
			break;
		
		case NDECLARATIONS:
			ast_free(ast->declarations.declarations);
    		ast_free(ast->declarations.declaration);
			free(ast);
			ast = NULL;
			break;

		case NSTATEMENTS:
			ast_free(ast->statements.statements);
    		ast_free(ast->statements.statement);
			free(ast);
			ast = NULL;
			break;

	/*4*/
	case NTYPE_DECLARATION:
		ast_free(ast->type_declaration.type);
    	free(ast->type_declaration.id);
		free(ast);
		ast = NULL;
   		break;

	case NASSIGN_DECLARATION:
		ast_free(ast->assign_declaration.type);
		free(ast->assign_declaration.id);
		ast_free(ast->assign_declaration.expression);
		free(ast);
		ast = NULL;
		break;

	case NCONST_DECLARATION:
		ast_free(ast->const_declaration.type);
		free(ast->const_declaration.id);
		ast_free(ast->const_declaration.expression);
		free(ast);
		ast = NULL;
		break;

	/*7*/
	case NASSIGN_STATEMENT:
		ast_free(ast->assign_statement.variable);
		ast_free(ast->assign_statement.expression);
		free(ast);
		ast = NULL;
		break;
	
	case NIF_STATEMENT:
		ast_free(ast->if_statement.condition);
		ast_free(ast->if_statement.statement);
		free(ast);
		ast = NULL;
		break;

	case NIF_ELSE_STATEMENT:
		ast_free(ast->if_else_statement.condition);
		ast_free(ast->if_else_statement.statement);
		ast_free(ast->if_else_statement.else_statement);
		free(ast);
		ast = NULL;
		break;

	case NSCOPE_STATEMENT:
		ast_free(ast->scope_statement.scope);
		free(ast);
		ast = NULL;
		break;
	
	/*11*/
	case NUNARY_EXPR:
    	ast_free(ast->unary_expr.right);
		free(ast);
		ast = NULL;
    	break;
		
	case NBINARY_EXPR:
    	ast_free(ast->binary_expr.left);
    	ast_free(ast->binary_expr.right);
    	break;

	case NBRACKETS_EXPR:
		ast_free(ast->brackets_expr.expression);
		free(ast);
		ast = NULL;
		break;

	case NFUNC_EXPR:
		ast_free(ast->func_expr.arguments_opt);
		free(ast);
		ast = NULL;
		break;
	
	case NTYPE_EXPR:
		ast_free(ast->type_expr.type);
		ast_free(ast->type_expr.arguments_opt);
		free(ast);
		ast = NULL;
		break;

	case NVAR_EXPR:
		ast_free(ast->var_expr.variable);
		free(ast);
		ast = NULL;
		break;

	case NINT_EXPR:
		free(ast);
		ast = NULL;
		break;

	case NFLOAT_EXPR:
		free(ast);
		ast = NULL;
		break;

	case NBOOL_EXPR:
		free(ast);
		ast = NULL;
		break;
	
	/*20*/
	case NID_VARIABLE:
		free(ast->id_variable.id);
		free(ast);
		ast = NULL;
		break;

	case NARRAY_VARIABLE:
		free(ast->array_variable.id);
		free(ast);
		ast = NULL;
		break;

	case NARGS_ARGUMENTS:
		ast_free(ast->args_arguments.arguments);
		ast_free(ast->args_arguments.expression);
		free(ast);
		ast = NULL;
		break;
	
	case NEXPR_ARGUMENTS:
		ast_free(ast->expr_arguments.expression);
		free(ast);
		ast = NULL;
		break;

	case NARGUMENTS_OPT:
		ast_free(ast->arguments_opt.arguments);
		free(ast);
		ast = NULL;
		break;
	
	case NTYPE:
		free(ast);
		ast = NULL;
		break;

	default:break;
	}
	
}

void ast_print(node * ast, int flag) {
	if(ast == NULL)
		return;
	
	int kind = ast->kind;
	int type;
	//a flag for variable printing
	//0: accessed in statement
	//1: accessed in expr
	switch(kind){
		
		case NSCOPE:
			printf("(SCOPE ");
			ast_print(ast->scope.declarations, 0);
			ast_print(ast->scope.statements, 0);
			printf(")\n");
			break;
		
		case NDECLARATIONS:
			printf("(DECLARATIONS ");
			ast_print(ast->declarations.declarations,0);
    		ast_print(ast->declarations.declaration,0);
			printf(")\n");
			break;

		case NSTATEMENTS:
			printf("(STATEMENTS ");
			ast_print(ast->statements.statements,0);
    		ast_print(ast->statements.statement,0);
			break;

	/*4*/
	case NTYPE_DECLARATION:
		printf("(DECLARATION %s ", ast->type_declaration.id);
		ast_print(ast->type_declaration.type,0);
		printf(")\n");
   		break;

	case NASSIGN_DECLARATION:
		printf("(DECLARATION %s ", ast->type_declaration.id);
		ast_print(ast->assign_declaration.type,0);
		ast_print(ast->assign_declaration.expression,0);
		printf(")\n");
		break;

	case NCONST_DECLARATION:
		printf("(CONST DECLARATION %s ", ast->type_declaration.id);
		ast_print(ast->const_declaration.type,0);
		ast_print(ast->const_declaration.expression,0);
		printf(")\n");
		break;

	/*7*/
	case NASSIGN_STATEMENT:
		printf("(ASSIGN ");
		ast_print(ast->assign_statement.variable,0);
		ast_print(ast->assign_statement.expression,0);
		printf(")\n");
		break;
	
	case NIF_STATEMENT:
		printf("(IF ");
		ast_print(ast->if_statement.condition,0);
		ast_print(ast->if_statement.statement,0);
		printf(")\n");
		break;

	case NIF_ELSE_STATEMENT:
		printf("(IF ");
		ast_print(ast->if_else_statement.condition,0);
		ast_print(ast->if_else_statement.statement,0);
		ast_print(ast->if_else_statement.else_statement,0);
		printf(")\n");
		break;

	case NSCOPE_STATEMENT:
		ast_print(ast->scope_statement.scope,0);
		break;
	
	/*11*/
	case NUNARY_EXPR:
		printf("(UNARY ");
		printOp(ast->unary_expr.op);
    	ast_print(ast->unary_expr.right,0);
		printf(")\n");
    	break;
		
	case NBINARY_EXPR:
		printf("(BINARY ");
		printOp(ast->binary_expr.op);
    	ast_print(ast->binary_expr.left,0);
    	ast_print(ast->binary_expr.right,0);
		printf(")\n");
    	break;

	case NBRACKETS_EXPR:
		ast_print(ast->brackets_expr.expression, 0);
		break;

	case NFUNC_EXPR:
		printf("(CALL ");
		if(ast->func_expr.func == 0){
			printf("dp3 ");
		}else if(ast->func_expr.func == 1){
			printf("lit ");
		}else if(ast->func_expr.func == 2){
			printf("rsq ");
		}
		ast_print(ast->func_expr.arguments_opt,0);
		printf(")\n");
		break;
	
	case NTYPE_EXPR:
		printf("(CALL ");
		ast_print(ast->type_expr.type,0);
		ast_print(ast->type_expr.arguments_opt,0);
		printf(")\n");
		break;

	case NVAR_EXPR:
		ast_print(ast->var_expr.variable,1);
		break;

	case NINT_EXPR:
		printf("<%d>\n", ast->int_expr.number);
		break;

	case NFLOAT_EXPR:
		printf("<%f>\n", ast->float_expr.number);
		break;

	case NBOOL_EXPR:
		printf("<bool ");
		if(ast->bool_expr.boolean == 0){
			printf("true>\n");
		}else if(ast->bool_expr.boolean == 1){
			printf("false>\n");
		}
		break;
	
	/*20*/
	case NID_VARIABLE:
		if(flag == 0)
		{
			/*type = get_symtable_type(ast->array_variable.id);
			printType(type);*/
			printf("%s ", ast->id_variable.id);
		}else{
			printf("<%s>\n", ast->id_variable.id);
		}
		break;

	case NARRAY_VARIABLE:
		if(flag == 0)
		{
			/*type = get_symtable_type(ast->array_variable.id);
			printType(type);*/
			printf("%s ", ast->id_variable.id);

		}else{
			printf("(INDEX ");
			/*type = get_symtable_type(ast->array_variable.id);
			if(type == IVEC2 | type == IVEC3 | type == IVEC4){
				printf("int "); 
			}else if(type == VEC2 | type == VEC3 | type == VEC4){
				printf("float ");
			}else if(type == BVEC2 | type == BVEC3 | type == BVEC4){
				printf("bool ");
			}*/
			printf("%s <%d>\n", ast->array_variable.id, ast->array_variable.index);
		}
		break;

	case NARGS_ARGUMENTS:
		ast_print(ast->args_arguments.arguments,0);
		ast_print(ast->args_arguments.expression,0);
		break;
	
	case NEXPR_ARGUMENTS:
		ast_print(ast->expr_arguments.expression, 0);
		break;

	case NARGUMENTS_OPT:
		ast_print(ast->arguments_opt.arguments, 0);
		break;
	
	case NTYPE:
		printType(ast->type.type_kind);
		break;

	default:break;
	}

}


void printType(int type){
	switch(type){
		case INT:
			printf("int\n");
			break;
		case  IVEC2:
			printf("ivec2\n");
			break;
		case IVEC3:
			printf("ivec3\n");
			break;
		case IVEC4:
			printf("ivec4\n");
			break;
		case BOOL:
			printf("bool\n");
			break;
		case BVEC2:
			printf("bvec2\n");
			break;
		case BVEC3:
			printf("bvec3\n");
			break;
		case BVEC4:
			printf("bvec4\n");
			break;
		case FLOAT:
			printf("float\n");
			break;
		case VEC2:
			printf("vec2\n");
			break;
		case VEC3:
			printf("vec3\n");
			break;
		case VEC4:
			printf("vec4\n");
			break;

	}

}


void printOp(int op){
	switch(op){
		case	AND_OPS:
			printf("AND && ");
			break;
		case OR_OPS:
			printf("OR || ");
			break;
		case EQ_OPS:
			printf("EQ == ");
			break;
		case NEQ_OPS:
			printf("NEQ != ");
			break;
		case GEQ_OPS:
			printf("GEQ >= ");
			break;
		case LEQ_OPS:
			printf("LEQ <= ");
			break;
		case LESS_OPS:
			printf("LESS < ");
			break;
		case GTR_OPS:
			printf("GTR > ");
			break;
		case PLUS_OPS:
			printf("PLUS + ");
			break;
		case MINUS_OPS:
			printf("MINUS - ");
			break;
		case TIMES_OPS:
			printf("TIMES * ");
			break;
		case DIVIDE_OPS:
			printf("DIVIDE / ");
			break;
		case POW_OPS:
			printf("POW ^ ");
			break;
		case NEG_OPS:
			printf("NEG - ");
			break;
		case NOT_OPS:
			printf("NOT ! ");
			break;
	}

}

