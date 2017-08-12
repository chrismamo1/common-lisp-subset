#include <iostream>

#include "cell.hpp"
#include "closure.hpp"
#include "util.hpp"

Cell::Cell()
{
        this->type = NIL;
}

Cell::Cell(S_Expression *sexp)
{
        this->type = SEXP;
        this->sexp = sexp;
}

Cell::Cell(Closure *closure)
{
        this->type = FUNC;
        this->closure = closure;
}

Cell::Cell(ConsCell cons)
{
        this->type = CONS;
        this->cons = cons;
}

Cell::Cell(double r)
{
        this->type = REAL;
        this->real = r;
}

Cell::Cell(BooleanValue b)
{
        this->type = BOOLEAN;
        this->boolean = b;
}

void Cell::replace(Cell c)
{
        this->type = c.type;
        switch (c.type) {
        case CONS:
                this->cons = c.cons;
                break;
        case FUNC:
                this->closure = c.closure;
                break;
        case SEXP:
                this->sexp = c.sexp;
                break;
        case REAL:
                this->real = c.real;
                break;
        case BOOLEAN:
                this->boolean = c.boolean;
                break;
        case NIL:
                break;
        }
}

bool Cell::equals(Cell c)
{
        if (c.type != this->type)
                return false;
        else
                switch (c.type) {
                case CONS:
                        return false;
                case FUNC:
                        return this->closure == c.closure;
                case SEXP:
                        return this->sexp == c.sexp;
                case REAL:
                        return this->real == c.real;
                case BOOLEAN:
                        return this->boolean == c.boolean;
                case NIL:
                        return true;
        }
        return false;
}

bool Cell::operator==(Cell c)
{
        return this->equals(c);
}

string Cell::toString()
{
        //string rval = "CELL TYPE: ";
        string rval = "";
        switch (this->type) {
        case CONS:
                //rval += "CONS\nVALUE: ";
                rval += "[cons (" + this->cons.car->toString() + ") ";
                if (this->cons.cdr == NULL)
                        rval += "(nil)]";
                else
                        rval += "(" + this->cons.cdr->toString() + ")]";
                return rval;
                break;
        case FUNC:
                //rval += "FUNC\nVALUE: ";
                rval += this->closure->toString();
                break;
        case SEXP:
                //rval += "SEXP\nVALUE: ";
                rval += this->sexp->toString();
                break;
        case REAL:
                //rval += "REAL\nVALUE: ";
                rval += to_string(this->real);
                break;
        case BOOLEAN:
                //rval += "BOOLEAN\nVALUE: ";
                switch (this->boolean) {
                case T:
                        rval += "T";
                        break;
                case F:
                        rval += "F";
                        break;
                }
                break;
        case NIL:
                rval += "NIL";
                break;
        }
        return rval;
}
