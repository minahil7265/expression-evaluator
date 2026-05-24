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

// ============================================================
// Part 1: Tokenizer
// ============================================================

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



int precedence(const string& op) {
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}

vector<Exp> infixToPostfix(const vector<Exp>& exps) {
    vector<Exp> output;
    stack<Exp>  opStack;
    for (const Exp& e : exps) {
        if (e.type == ExpType::NUMBER || e.type == ExpType::VARIABLE) {
            output.push_back(e);
        } else if (e.type == ExpType::OPERATOR) {
            while (!opStack.empty() &&
                   opStack.top().type == ExpType::OPERATOR &&
                   precedence(opStack.top().value) >= precedence(e.value)) {
                output.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(e);
        } else if (e.type == ExpType::LPAREN) {
            opStack.push(e);
        } else if (e.type == ExpType::RPAREN) {
            char closeChar = e.value[0];
            char openChar  = (closeChar==')') ? '(' :
                             (closeChar==']') ? '[' : '{';
            bool found = false;
            while (!opStack.empty()) {
                Exp top = opStack.top(); opStack.pop();
                if (top.type == ExpType::LPAREN) {
                    if (top.value[0] != openChar) {
                        cerr << "Syntax error: mismatched brackets.\n";
                        exit(1);
                    }
                    found = true; break;
                }
                output.push_back(top);
            }
            if (!found) {
                cerr << "Syntax error: unmatched closing bracket.\n";
                exit(1);
            }
        }
    }
    while (!opStack.empty()) {
        Exp top = opStack.top(); opStack.pop();
        if (top.type == ExpType::LPAREN) {
            cerr << "Syntax error: unmatched opening bracket.\n";
            exit(1);
        }
        output.push_back(top);
    }
    return output;
}
