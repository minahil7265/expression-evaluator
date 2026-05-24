#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include <cctype>
using namespace std;


enum class ExpType {
    NUMBER, VARIABLE, OPERATOR, LPAREN, RPAREN
};

struct Exp {
    ExpType type;
    string  value;
    double  number;

    Exp(ExpType t, string v) : type(t), value(v), number(0.0) {}
};


vector<Exp> tokenize(const string& expr) {
    vector<Exp> exps;
    size_t i = 0, n = expr.size();

    while (i < n) {
        if (isspace(expr[i])) { ++i; continue; }
        char c = expr[i];

        if (isdigit(c)) {
            string num;
            while (i < n && isdigit(expr[i])) num += expr[i++];
            if (i < n && expr[i] == '.') {
                cerr << "Syntax error: floating-point not supported.\n";
                exit(1);
            }
            Exp e(ExpType::NUMBER, num);
            e.number = stod(num);
            exps.push_back(e);
            continue;
        }

        if (isalpha(c) || c == '_') {
            string id;
            while (i < n && (isalnum(expr[i]) || expr[i] == '_')) id += expr[i++];
            exps.emplace_back(ExpType::VARIABLE, id);
            continue;
        }

        if (c=='+' || c=='-' || c=='*' || c=='/') {
            exps.emplace_back(ExpType::OPERATOR, string(1,c));
            ++i; continue;
        }
        if (c=='(' || c=='[' || c=='{') {
            exps.emplace_back(ExpType::LPAREN, string(1,c));
            ++i; continue;
        }
        if (c==')' || c==']' || c=='}') {
            exps.emplace_back(ExpType::RPAREN, string(1,c));
            ++i; continue;
        }

        cerr << "Syntax error: unexpected character '" << c << "'.\n";
        exit(1);
    }
    return exps;
}