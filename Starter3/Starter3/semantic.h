#ifndef _SEMANTIC_H
#define _SEMANTIC_H

#include <assert.h>
#include "ast.h"
#include "symbol.h"

int semantic_check( node *ast);
int CheckTypes(int type1, int type2);
int vectorCompare(int type1, int type2);
int vectorChecking(int type);

#endif
