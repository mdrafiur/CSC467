
#ifndef AST_H_
#define AST_H_ 1

#include <stdarg.h>

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
	UNKNOWN  
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
	NTYPE
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
}node_type;

typedef enum{
	AND,
	OR,
	EQ,
	NEQ,
	GEQ,
	LEQ,
	LESS,
	GTR,
	PLUS,
	MINUS,
	TIMES,
	DIVIDE,
	POW,
	NEG,
	NOT
}ops;


struct node_ {

  // an example of tagging each node with a type
  node_kind kind;

  union {
    struct {
       node *declarations ;
       node *statements;
    } scope;

    struct{
		node* declarations;
		node* declaration;
	}declarations;

	struct {
		node* statements;
		node* statement;
	}statements;
	
	struct{
		node* type;
		char* id;
	}type_declaration;
	
	struct{
		int type_kind;
	}type;

	struct{
		node* type;
		char* id;
		node* expression;
	}assign_declaration;

	struct{
		node* type;
		char* id;
		node* expression;
	}const_declaration;

	struct{
		node* variable;
		node* expression;
	}assign_statement;

	struct{
		node* codition;
		node* statement;
	}if_statement;

	struct{
		node* condition;
		node* statement;
		node* else_statement;
	}if_else_statement;

	struct{
		node* scope;
	}scope_statement;

	struct {
      int op;
      node *right;
    } unary_expr;

    struct {
      int op;
      node *left;
      node *right;
    } binary_expr;
	
	struct{
		node* expression;
	}brackets_expr;
	
	struct{
		int func;
		node* arguments_opt;
	}func_expr;
	
	struct{
		node* type;
		node* arguments_opt;
	}type_expr;
	
	struct{
		node* variable;
	}var_expr;
	
	struct{
		int number;
	}int_expr;

	struct{
		double number;
	}float_expr;

	struct{
		/*0 for true, 1 for false*/
		int boolean;
	}bool_expr;
	
	struct{
		char* id;
	}id_variable;
	
	struct{
		char* id;
		int index;
	}array_variable;


	struct{
		node* arguments;
		node* expression;
	}args_arguments;

	struct{
		node* expression;
	}expr_arguments;

	struct{
		node* arguments;
	}arguments_opt;
	
	struct{
		int type_kind;
	}type;



  };
};

node *ast_allocate(node_kind type, ...);
void ast_free(node *ast);
void ast_print(node * ast, int flag);

#endif /* AST_H_ */
