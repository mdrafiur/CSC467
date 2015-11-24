#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "symbol.h"
#include "ast.h"
#include "symtable.h"

void symtable_add(node *ast) {

  assert(ast);

	switch((int)ast->kind){
		case 0:
			break;
			
		case 1:
			build_table(ast->scope.declarations);
			build_table(ast->scope.statements);
			break;
			
		case 2:
			build_table(ast->declarations.declarations);
			build_table(ast->declarations.declaration);
			break;
			
		case 3:
			build_table(ast->statements.statements);
			build_table(ast->statements.statement);
			break;
			
		case 4:
		  build_table(ast->type_declaration.type);
			build_table(ast->type_declaration.id);
			break;
			
		case 5:
		  build_table(ast->assign_declaration.type);
			build_table(ast->assign_declaration.id);
			build_table(ast->assign_declaration.expression);
			break;
			
		case 6:
		  build_table(ast->const_declaration.type);
			build_table(ast->const_declaration.id);
			build_table(ast->const_declaration.expression);
			break;
			
		case 7:
		  build_table(ast->assign_statement.variable);
			build_table(ast->assign_statement.expression);
			break;
		case 9:
			break;
		case 10:
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			break;
		case 14:
			break;
