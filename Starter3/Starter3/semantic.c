#include "semantic.h"

int num_scope = 0;
int right_expr, left_expr;

int semantic_check( node *ast) {
  
    assert(ast);

    switch((int)ast->kind){
        case 0:
            break;

        case 1:
          right_expr = semantic_check(ast->scope.declarations);
			    left_expr = semantic_check(ast->scope.statements);
			    
			    if(right_expr && left_expr)
			      return 0;
			      
			     
			    
  return 0; // failed checks
}
