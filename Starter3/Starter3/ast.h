
#ifndef AST_H_
#define AST_H_ 1

#include <stdarg.h>
#include "symtable.h"

// Dummy node just so everything compiles, create your own node/nodes
//
// The code provided below is an example ONLY. You can use/modify it,
// but do not assume that it is correct or complete.
//
// There are many ways of making AST nodes. The approach below is an example
// of a descriminated union. If you choose to use C++, then I suggest looking
// into inheritance.

// forward declare
struct node_;
typedef struct node_ node;
extern node *ast;

typedef enum {
	UNKNOWN, 
	NSCOPE,   
	NDECLARATIONS,
	NSTATEMENTS,

	/*4*/
	NTYPE_DECLARATION,
	NASSIGN_DECLARATION,
	NCONST_DECLARATION,

	/*7*/
	NASSIGN_STATEMENT,
	NIF_STATEMENT,
	NIF_ELSE_STATEMENT,
	NSCOPE_STATEMENT,

	/*11*/
	NUNARY_EXPR,
	NBINARY_EXPR,
	NBRACKETS_EXPR,
	NFUNC_EXPR,
	NTYPE_EXPR,
	NVAR_EXPR,
	NINT_EXPR,
	NFLOAT_EXPR,
	NBOOL_EXPR,
	
	/*20*/
	NID_VARIABLE,
	NARRAY_VARIABLE,
	NARGS_ARGUMENTS,
	NEXPR_ARGUMENTS,
	NARGUMENTS_OPT,
	NTYPE,
	NPROG_SCOPE,
} node_kind;

typedef enum{
    INT,
    IVEC2,
    IVEC3,
    IVEC4,
    BOOL,
    BVEC2,
    BVEC3,
    BVEC4,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    FUNCTION,
}node_type;

typedef enum{
	AND_OPS,
	OR_OPS,
	EQ_OPS,
	NEQ_OPS,
	GEQ_OPS,
	LEQ_OPS,
	LESS_OPS,
	GTR_OPS,
	PLUS_OPS,
	MINUS_OPS,
	TIMES_OPS,
	DIVIDE_OPS,
	POW_OPS,
	NEG_OPS,
	NOT_OPS
}ops;


struct node_ {

  // an example of tagging each node with a type
  node_kind kind;

  union {

	struct{
		node* scope;
	}prog_scope;

    struct {
       node *declarations ;
       node *statements;
		int l;
    } scope;

    struct{
		node* declarations;
		node* declaration;
		int l;
	}declarations;

	struct {
		node* statements;
		node* statement;
		int l;
	}statements;
	
	struct{
		node* type;
		char* id;
		int l;
	}type_declaration;
	
	struct{
		int type_kind;
		int l;
	}type;

	struct{
		node* type;
		char* id;
		node* expression;
		int l;
	}assign_declaration;

	struct{
		node* type;
		char* id;
		node* expression;
		int l;
	}const_declaration;

	struct{
		node* variable;
		node* expression;
		int l;
	}assign_statement;

	struct{
		node* condition;
		node* statement;
		int l;
	}if_statement;

	struct{
		node* condition;
		node* statement;
		node* else_statement;
		int l;
	}if_else_statement;

	struct{
		node* scope;
		int l;
	}scope_statement;

	struct {
      int op;
      node *right;
	 int l;
	int type;
    } unary_expr;

    struct {
      int op;
      node *left;
      node *right;
		int l;
	  int type;
    } binary_expr;
	
	struct{
		node* expression;
		int l;
	}brackets_expr;
	
	struct{
		int func;
		node* arguments_opt;
		int l;
	}func_expr;
	
	struct{
		node* type;
		node* arguments_opt;
		int l;
	}type_expr;
	
	struct{
		node* variable;
		int l;
	}var_expr;
	
	struct{
		int number;
		int l;
	}int_expr;

	struct{
		double number;
		int l;
	}float_expr;

	struct{
		/*0 for true, 1 for false*/
		int boolean;
		int l;
	}bool_expr;
	
	struct{
		char* id;
		int l;
	}id_variable;
	
	struct{
		char* id;
		int index;
		int l;
	}array_variable;


	struct{
		node* arguments;
		node* expression;
		int l;
	}args_arguments;

	struct{
		node* expression;
		int l;
	}expr_arguments;

	struct{
		node* arguments;
		int l;
	}arguments_opt;
	
	


  };
};

node *ast_allocate(node_kind type, ...);
void ast_free(node *ast);
void ast_print(node * ast, int flag);
void printType(int type);
void printOp(int op);

#endif /* AST_H_ */
