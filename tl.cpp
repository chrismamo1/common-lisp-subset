#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <ctype.h>
#include <unordered_map>

#include <unistd.h>

#include "s_expression.hpp"
#include "closure.hpp"
#include "cell.hpp"

using namespace std;

enum LispValType { FLOAT, INT, STRING, SYMBOL, UNKNOWN };
enum ParserState_e { P_NONE = 1, P_SEXP = 2, P_STRN = 4, P_NUMB = 8, P_SYMB = 16 };
enum SExpTag_e { T_NORMAL = 1, T_DO_NOT_EXECUTE = 2, T_PLACEHOLDER_SO_I_REMEMBER_TO_MAKE_THESE_POWERS_OF_TWO = 4 };

string pstate_to_string(ParserState_e pstate)
{
        switch (pstate) {
        case P_NONE:    return "P_NONE";
        case P_SEXP:    return "P_SEXP";
        case P_STRN:    return "P_STRN";
        case P_NUMB:    return "P_NUMB";
        case P_SYMB:    return "P_SYMB";
        default:        return NULL;    }
}

string parseSymb(string::iterator *it, string::iterator it_end)
{
        string acc = "";
        while (*it < it_end) {
                if (isalnum(**it) || **it == '-' || **it == '_' || **it == '+' || **it == '/' || **it == '*' || **it == '=' || **it == '!' || **it == '>' || **it == '<' || **it == '.' || **it == '-' || **it == '?') {
                        acc += tolower(**it);
                } else if (**it == ')'){
                        (*it)--;
                        return acc;
                } else {
                        break;
                }
                ++(*it);
        }
        return acc;
}

/** the 1st black box, i.e. `read` */
S_Expression parseSexp(string::iterator *it, string::iterator it_end)
{
        S_Expression *rval = new S_Expression();
        string acc;
        while (**it != '(')
                ++(*it);
        ++(*it);
        do {
                switch (**it) {
                case '(':
                        rval->addChild(parseSexp(it, it_end));
                        break;
                case ' ':
                        break;
                case ')':
                        return *rval;
                default:
                        string s = parseSymb(it, it_end);
                        rval->addChild(*(new S_Expression(s)));
                        if (*it >= it_end)
                                throw "Syntax error: probably missing a ``)''.";
                        break;
                }
                (*it) = (*it) + 1;
        } while (*it < it_end);
        throw "Syntax error: probably missing a ``)''.";
        return *rval;
}

S_Expression read(string::iterator *it, string::iterator it_end)
{
        while (**it == ' ')
                ++(*it);
        if (**it == '(')
                try {
                        return parseSexp(it, it_end);
                } catch (char const* exn) {
                        cout << "failure: " << exn << endl;
                        exit(1);
                }
        else {
                string s = parseSymb(it, it_end);
                S_Expression *sexp = new S_Expression(s);
                return *sexp;
        }
}

int main (int argc, char *argv[]) {
        ifstream init_file;
        ofstream out_file;
        S_Expression tmp;
        string buf, acc = "";
        stack<ParserState_e> pstate;
        stack<S_Expression> sexps;
        pstate.push(P_NONE);
        unordered_map<string, S_Expression> env;
        Closure *c = NULL;
        cout << "\\> ";
        out_file.open("results.file");
        for (int i = 1; i < argc; i++) {
                init_file.open(argv[i]);
                while (getline(init_file, buf)) {
                        if (buf == "" || buf == "\n")
                                continue;
                        cout << buf << "\n";
                        string::iterator it = buf.begin();
                        tmp = read(&it, buf.end());
                        c = new Closure(tmp, c);
                        string s = (c->eval())->toString();
                        cout << s << "\n";
                        cout << "\\> ";
                        out_file << s << "\n";
                }
                init_file.close();
        }
        while (getline(cin, buf)) {
                string::iterator it = buf.begin();
                tmp = read(&it, buf.end());
                c = new Closure(tmp, c);
                string s = c->eval()->toString();
                cout << s << "\n";
                cout << "\\> ";
                out_file << s << "\n";
        }
        cout << "Goodbye!\n";
        out_file.close();

        return 0;
}
