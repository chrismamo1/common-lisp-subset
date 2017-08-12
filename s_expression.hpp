#ifndef _S_EXPRESSION_H
#define _S_EXPRESSION_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;

enum SExpType_e { T_LEAF, T_NODE };

class S_Expression {
        vector<S_Expression> children;
        string value = "";
        SExpType_e type;
public:
        S_Expression() {
                value = " ";
                type = T_NODE;
        }

        S_Expression(string fst) {
                value = fst;
                type = T_LEAF;
        }

        SExpType_e getType();

        void setValue(string v);

        string getValue();

        void addChild(S_Expression v);

        vector<S_Expression> getChildren();

        string toString();

        bool equals(S_Expression sexp);
};

#endif
