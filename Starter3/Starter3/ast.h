
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
  UNKNOWN               = 0,

  SCOPE_NODE            = (1 << 0),
  
  EXPRESSION_NODE       = (1 << 2),
  UNARY_EXPRESION_NODE  = (1 << 2) | (1 << 3),
  BINARY_EXPRESSION_NODE= (1 << 2) | (1 << 4),
  INT_NODE              = (1 << 2) | (1 << 5), 
  FLOAT_NODE            = (1 << 2) | (1 << 6),
  IDENT_NODE            = (1 << 2) | (1 << 7),
  VAR_NODE              = (1 << 2) | (1 << 8),
  FUNCTION_NODE         = (1 << 2) | (1 << 9),
  CONSTRUCTOR_NODE      = (1 << 2) | (1 << 10),

  STATEMENT_NODE        = (1 << 1),
  IF_STATEMENT_NODE     = (1 << 1) | (1 << 11),
  WHILE_STATEMENT_NODE  = (1 << 1) | (1 << 12),
  ASSIGNMENT_NODE       = (1 << 1) | (1 << 13),
  NESTED_SCOPE_NODE     = (1 << 1) | (1 << 14),

  DECLARATION_NODE      = (1 << 15)
} node_kind;



typedef enum{
	INT_T,
    IVEC_T,
	BOOL_T,
	BVEC_T,
	FLOAT_T,
	VEC_T
}node_type;




struct node_ {

  // an example of tagging each node with a type
  node_kind kind;

  union {
    struct {
       node *declarations ;
       node *statements;
    } scope;
  
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
		node* declarations;
		node* declaration;
	}declarations;

	struct {
		node* statements;
		node* statement;
	}statements;

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

	struct{
		node* expression;
	}brackets_expr;
	

	struct{
		node_type type;
		char* id;
	}type_declaration;

	struct{
		node_type type;
		char* id;
		node* expression;
	}assign_declaration;

	struct{
		node_type type;
		char* id;
		node* expression;
	}const_declaration;

	struct{
		int func;
		node* arguments_opt;
	}func_expr;
	
	struct{
		node_type type;
		node* arguments_opt;
	}type_expr;
	
	struct{
		node* variable;
	}var_expression;
	
	struct{
		int number;
	}int_expr;

	struct{
		float number;
	}float_expr;

	struct{
		bool boolean;
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



  };
};

node *ast_allocate(node_kind type, ...);
void ast_free(node *ast);
void ast_print(node * ast);

#endif /* AST_H_ */
