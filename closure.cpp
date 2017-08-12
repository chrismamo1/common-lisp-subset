#include <iostream>

#include "decls.hpp"
#include "closure.hpp"
#include "cell.hpp"
#include "util.hpp"

Closure::Closure(S_Expression lambda, Closure *_parent)
{
        this->expr = lambda;
        this->parent = _parent;
}

Cell* Closure::getBinding(string name)
{
        if (this->bindings.count(name) > 0)
                return this->bindings[name];
        else {
                if (this->parent == NULL)
                        return NULL;
                else
                        return this->parent->getBinding(name);
        }
}

void Closure::setBinding(string name, Cell *value)
{
        this->bindings[name] = value;
}

void Closure::eagerlySetBinding(string name, Cell *value)
{
        Cell* binding = this->getBinding(name);
        if (binding == NULL)
                this->bindings[name] = value;
        else
                binding->replace(*value);
}

Cell* Closure::eval()
{
        return this->eval(this->expr);
}

Cell* Closure::eval(S_Expression sexp)
{
        string cmd = "";
        if (sexp.getType() == T_LEAF) {
                if (sexp.getValue() == "t")
                        return new Cell(T);
                else if (sexp.getValue() == "f")
                        return new Cell(F);
                else if (sexp.getValue() == "nil")
                        return new Cell();
                Cell *rv = this->getBinding(sexp.getValue());
                if (rv == NULL)
                        return new Cell(stod(sexp.getValue()));
                else {
                        return rv;
                }
        }

        auto kids = sexp.getChildren();
        if (kids[0].getType() != T_LEAF) {
                Closure *clos = new Closure(kids[0], this);
                auto func = clos->eval(kids[0]);
                Closure *caller = func->closure;

                auto param_names = caller->expr.getChildren()[1].getChildren();
                for (int i = 0, j = 1; i < param_names.size(); ++i, ++j) {
                        auto val = this->eval(kids[j]);
                        caller->setBinding(param_names[i].getValue(), val);
                }
                return caller->eval(caller->expr.getChildren()[2]);
        } else
                cmd = kids[0].getValue();

        if (cmd == "define") {
                string bname = kids[1].getValue();
                Cell *val = this->eval(kids[2]);
                this->setBinding(bname, val);
                return this->getBinding(bname);
        } else if (cmd == "begin") {
                Cell *rval = new Cell(NIL);
                for (int i = 1; i < kids.size(); i++) {
                        rval = this->eval(kids[i]);
                }
                return rval;
        } else if (cmd == "set!") {
                string bname = kids[1].getValue();
                Cell *val = this->eval(kids[2]);
                this->eagerlySetBinding(bname, val);
                return this->getBinding(bname);
        } else if (cmd == "quote") {
                return new Cell(&kids[1]);
        } else if (cmd == "eval") {
                S_Expression *s = (this->eval(kids[1]))->sexp;
                return this->eval(*s);
        } else if (cmd == "lambda") {
                return new Cell(new Closure(sexp, this));
        } else if (cmd == "if") {
                Cell *cond = this->eval(kids[1]);
                if (cond->type == BOOLEAN && cond->boolean == T)
                        return this->eval(kids[2]);
                else
                        return this->eval(kids[3]);
        } else if (cmd == "plus" || cmd == "minus" || cmd == "times" || cmd == "over") {
                float o1 = this->eval(kids[1])->real;
                float o2 = this->eval(kids[2])->real;
                switch (cmd[0]) {
                case 'p':
                        return new Cell(o1 + o2);
                case 'm':
                        return new Cell(o1 - o2);
                case 't':
                        return new Cell(o1 * o2);
                case 'o':
                        return new Cell(o1 / o2);
                }
                        
        } else if (cmd == "=") {
                if (this->eval(kids[1])->equals(*(this->eval(kids[2])))) {
                        return new Cell(T);
                }
                else {
                        return new Cell(F);
                }
        } else if (cmd == "ispos") {
                double o1 = this->eval(kids[1])->real;
                if (o1 > 0.0)
                        return new Cell(T);
                else
                        return new Cell(F);
        } else if (cmd == "cons") {
                ConsCell cell;
                cell.car = this->eval(kids[1]);
                cell.cdr = this->eval(kids[2]);
                return new Cell(cell);
        } else if (cmd == "_car") {
                ConsCell cell = this->eval(kids[1])->cons;
                return cell.car;
        } else if (cmd == "_cdr") {
                ConsCell cell = this->eval(kids[1])->cons;
                return cell.cdr;
        } else if (cmd == "list") {
                auto *cons = new Cell(*(new ConsCell));
                int i;
                Cell *head = cons;
                for (i = 1; i < kids.size() - 1; i++) {
                        cons->cons.car = this->eval(kids[i]);
                        cons->cons.cdr = new Cell(*(new ConsCell));
                        cons = cons->cons.cdr;
                }
                cons->cons.car = this->eval(kids[i]);
                cons->cons.cdr = new Cell();
                return head;
        } else if (this->getBinding(cmd) != NULL || cmd == "") {
                Closure *clos = this->getBinding(cmd)->closure;
#ifdef DYNAMIC
                Closure intermediate = *this;
                Closure *p;
                unordered_map<string, Cell*> inherited;
                for (p = intermediate.parent; p->parent != NULL; p = p->parent)
                        ;
                for (Closure *c = clos; c != NULL; c = c->parent)
                        for (auto it = c->bindings.begin(); it != c->bindings.end(); ++it)
                                inherited.insert(*it);
                p->parent = new Closure(clos->expr, NULL);
                p->parent->bindings = inherited;
                Closure *caller = new Closure(clos->expr, &intermediate);
                cout << caller->toString() << "\n\n";
#else
                Closure *caller = new Closure(clos->expr, clos);
#endif
                auto param_names = clos->expr.getChildren()[1].getChildren();
                for (int i = 0, j = 1; i < param_names.size(); ++i, ++j) {
                        auto val = this->eval(kids[j]);
                        caller->setBinding(param_names[i].getValue(), val);
                }
                std::cout << caller->expr.toString() << endl;
                return caller->eval(clos->expr.getChildren()[2]);
        } else {
                return new Cell(&(this->expr));
        }
        std::cout << this->toString() << endl;
        return NULL;
}

string Closure::toString()
{
        unordered_map<string, Cell*> inherited;
        string rval = this->expr.toString() + "\n\tLOCAL BINDINGS: [";

        for (auto it = this->bindings.begin(); it != this->bindings.end(); ++it)
                rval += it->first + " ";
        trim(rval);
        rval += "]\n";
        rval += "\tINHERITED BINDINGS: [";
        for (Closure *c = this->parent; c != NULL; c = c->parent)
                for (auto it = c->bindings.begin(); it != c->bindings.end(); ++it)
                        inherited.insert(*it);
        for (auto it = inherited.begin(); it != inherited.end(); ++it)
                rval += it->first + " ";
        trim(rval);
        rval += "]";
        return rval;
}
