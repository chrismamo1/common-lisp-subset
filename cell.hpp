#ifndef _CELL_H
#define _CELL_H

#include "decls.hpp"
#include "s_expression.hpp"

enum CellType {
        CONS, FUNC, SEXP, REAL, BOOLEAN, NIL
};

enum BooleanValue {
        T, F
};

typedef struct {
        Cell *car;
        Cell *cdr;
} ConsCell;

class Cell {
private:
public:
        CellType type;
        union {
                S_Expression *sexp;
                Closure *closure;
                ConsCell cons;
                double real;
                BooleanValue boolean;
        } ;
        Cell();
        Cell(S_Expression *sexp);
        Cell(Closure *closure);
        Cell(ConsCell cons);
        Cell(double r);
        Cell(BooleanValue b);
        CellType getType();
        void replace(Cell c);
        bool equals(Cell c);
        bool operator==(Cell c);
        string toString();
};
#endif
