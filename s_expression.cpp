#include "s_expression.hpp"

SExpType_e S_Expression::getType() {
        return type;
}

void S_Expression::setValue(string v) {
        if (type != T_LEAF)
                throw "cannot set the `value` of a non-leaf S-expression";
        value = v;
}

string S_Expression::getValue() {
        if (type != T_LEAF)
                throw "cannot get the `value` of a non-leaf S-expression";
        return value;
}

void S_Expression::addChild(S_Expression v) {
        if (type != T_NODE)
                throw "cannot add a child to a non-node S-expression";
        children.push_back(v);
}

vector<S_Expression> S_Expression::getChildren() {
        if (type != T_NODE)
                throw "cannot get children from a non-node S-expression";
        return children;
}

string S_Expression::toString() {
        string rval = "";
        if (this->getType() == T_LEAF)
                return this->value;
        else if (this->getType() == T_NODE) {
                rval += " ( ";
                auto kids = this->getChildren();
                for (int i = 0; i < kids.size(); i++)
                        rval += kids[i].toString();
                rval += " ) ";
        }
        return rval;
}

bool S_Expression::equals(S_Expression sexp) {
        if (type == T_LEAF && sexp.getType() == T_LEAF) {
                if (value == sexp.getValue())
                        return true;
        } else if (type == T_NODE && sexp.getType() == T_NODE) {
                auto it = children.begin();
                auto it2 = sexp.getChildren().begin();
                while (it != children.end() && it2 != sexp.getChildren().end()) {
                        if (!(*it).equals(*it2))
                                return false;
                        ++it;
                        ++it2;
                }
                return true;
        }
        return false;
}

