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

// ============================================================
// Part 4: Expression Validator
// ============================================================

void validateExps(const vector<Exp>& exps) {
    if (exps.empty()) {
        cerr << "Syntax error: empty expression.\n"; exit(1);
    }
    if (exps.front().type == ExpType::OPERATOR) {
        cerr << "Syntax error: expression cannot begin with operator.\n"; exit(1);
    }
    if (exps.back().type == ExpType::OPERATOR) {
        cerr << "Syntax error: expression cannot end with operator.\n"; exit(1);
    }
    for (size_t i = 1; i < exps.size(); ++i) {
        const Exp& prev = exps[i-1];
        const Exp& curr = exps[i];
        bool prevOperand = (prev.type==ExpType::NUMBER||prev.type==ExpType::VARIABLE||prev.type==ExpType::RPAREN);
        bool currOperand = (curr.type==ExpType::NUMBER||curr.type==ExpType::VARIABLE||curr.type==ExpType::LPAREN);
        if (prevOperand && currOperand) {
            cerr << "Syntax error: missing operator between '" << prev.value << "' and '" << curr.value << "'.\n"; exit(1);
        }
        if (prev.type==ExpType::OPERATOR && curr.type==ExpType::OPERATOR) {
            cerr << "Syntax error: consecutive operators.\n"; exit(1);
        }
        if (prev.type==ExpType::LPAREN && curr.type==ExpType::OPERATOR) {
            cerr << "Syntax error: operator after opening bracket.\n"; exit(1);
        }
        if (prev.type==ExpType::OPERATOR && curr.type==ExpType::RPAREN) {
            cerr << "Syntax error: operator before closing bracket.\n"; exit(1);
        }
        if (prev.type==ExpType::LPAREN && curr.type==ExpType::RPAREN) {
            cerr << "Syntax error: empty brackets.\n"; exit(1);
        }
    }
}

// ============================================================
// Part 5: Variable Collector + Postfix Evaluator + Main
// ============================================================

vector<string> collectVariables(const vector<Exp>& exps) {
    set<string> seen; vector<string> order;
    for (const Exp& e : exps)
        if (e.type==ExpType::VARIABLE && !seen.count(e.value)) {
            seen.insert(e.value); order.push_back(e.value);
        }
    return order;
}

double evaluatePostfix(const vector<Exp>& postfix,
                       const vector<double>& varVals,
                       const vector<string>& vars) {
    stack<double> stk;
    for (const Exp& e : postfix) {
        if (e.type==ExpType::NUMBER) {
            stk.push(e.number);
        } else if (e.type==ExpType::VARIABLE) {
            double val = 0.0;
            for (size_t i = 0; i < vars.size(); ++i)
                if (vars[i]==e.value) { val=varVals[i]; break; }
            stk.push(val);
        } else if (e.type==ExpType::OPERATOR) {
            if (stk.size() < 2) { cerr << "Logical error: insufficient operands.\n"; exit(3); }
            double rhs = stk.top(); stk.pop();
            double lhs = stk.top(); stk.pop();
            if      (e.value=="+") stk.push(lhs+rhs);
            else if (e.value=="-") stk.push(lhs-rhs);
            else if (e.value=="*") stk.push(lhs*rhs);
            else if (e.value=="/") {
                if (rhs==0.0) { cerr << "Runtime error: division by zero.\n"; exit(2); }
                stk.push(lhs/rhs);
            }
        }
    }
    if (stk.size()!=1) { cerr << "Logical error: malformed expression.\n"; exit(3); }
    return stk.top();
}

int main() {
    string line;
    if (!getline(cin, line)) { cerr << "Error: no input provided.\n"; return 1; }

    vector<Exp> exps = tokenize(line);
    validateExps(exps);

    vector<Exp> postfix = infixToPostfix(exps);

    for (size_t i = 0; i < postfix.size(); ++i) {
        if (i > 0) cout << ' ';
        cout << postfix[i].value;
    }
    cout << "\n";

    vector<string> vars = collectVariables(exps);
    vector<double> varVals(vars.size());

    if (!vars.empty()) {
        for (size_t i = 0; i < vars.size(); ++i) {
            cerr << "Enter value for " << vars[i] << ": ";
            if (!(cin >> varVals[i])) { cerr << "Error: invalid value.\n"; exit(3); }
        }
    }

    double result = evaluatePostfix(postfix, varVals, vars);

    if (result == static_cast<long long>(result))
        cout << static_cast<long long>(result) << "\n";
    else
        cout << result << "\n";

    return 0;
}
