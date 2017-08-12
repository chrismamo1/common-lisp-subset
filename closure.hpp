#include <unordered_map>

#ifndef _CLOSURE_H
#define _CLOSURE_H

#include "decls.hpp"
#include "s_expression.hpp"

class Closure {
public:
        Closure *parent = NULL;
        unordered_map<string, Cell*> bindings;
        S_Expression expr;
        Closure(S_Expression lambda, Closure *_parent);

        Cell *getBinding(string name);

        void setBinding(string name, Cell *value);
        
        /** sets the binding identified by `name`, eagerly
         * searching up the call chain to replace any
         * existing values
         */
        void eagerlySetBinding(string name, Cell *value);

        Cell *eval();

        Cell *eval(S_Expression sexp);

        string toString();
};
#endif
