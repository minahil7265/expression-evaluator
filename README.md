# expression-evaluator
 Infix to Postfix Expression Evaluator
 # Expression Evaluator

A C++ program that converts infix expressions to postfix
and evaluates them.

## Compilation
g++ -o expr expr.cpp -std=c++17

## Usage
./expr
Then type your expression: 3 + 5 * 2

## What This Program Does

This program takes a **math expression** typed by the user, converts it from
**Infix to Postfix** (Reverse Polish Notation), then **evaluates** it.
If the expression contains variables (like `a`, `b`, `x`), it asks the user
to enter values for them before calculating.

---

## How to Compile

```bash
g++ -o expr expr.cpp -std=c++17
```

---

## How to Run

```bash
./expr
```

Then type your expression and press ENTER:

```
3 + 5 * 2
```

**Output:**
```
3 5 2 * +       <- postfix form
13              <- result
```

---

## Supported Features

| Feature | Example |
|---------|---------|
| Basic operators | `+`  `-`  `*`  `/` |
| Parentheses (all 3 types) | `(`, `[`, `{` |
| Integer numbers | `3`, `100`, `999` |
| Variables | `a`, `x`, `myVar` |
| Operator precedence | `*` and `/` before `+` and `-` |
| Mixed bracket types | `{3 + [5 * 2]}` |

---

## Variable Example

```
a + b * 2
```

Program will ask:
```
Enter value for a: 3
Enter value for b: 4
```

Output:
```
a b 2 * +
11
```

---

## Error Handling

| Error | Example Input | Message |
|-------|--------------|---------|
| Floating point | `3.5 + 2` | `Syntax error: floating-point literals not supported.` |
| Empty expression | *(blank)* | `Syntax error: empty expression.` |
| Starts with operator | `* 3 + 2` | `Syntax error: expression cannot begin with operator.` |
| Ends with operator | `3 + 2 *` | `Syntax error: expression cannot end with operator.` |
| Two operators in a row | `3 + * 2` | `Syntax error: consecutive operators.` |
| Missing operator | `3 5 + 2` | `Syntax error: missing operator between '3' and '5'.` |
| Empty brackets | `3 + ()` | `Syntax error: empty brackets '()'.` |
| Mismatched brackets | `(3 + 5]` | `Syntax error: mismatched brackets.` |
| Unmatched open bracket | `(3 + 5` | `Syntax error: unmatched opening bracket.` |
| Unmatched close bracket | `3 + 5)` | `Syntax error: unmatched closing bracket.` |
| Division by zero | `5 / 0` | `Runtime error: division by zero.` |
| Unknown character | `3 @ 2` | `Syntax error: unexpected character '@'.` |

---

## Known Limitation

**Unary minus is NOT supported.**
`-5 + 3` will give a syntax error because `-` at the start is treated as
an operator, not a negative sign. Write `0 - 5 + 3` as a workaround.

---

## How Infix to Postfix Works (Shunting Yard Algorithm)

```
Input:   3 + 5 * 2
         ↓
Step 1:  3           → output: [3]
Step 2:  +           → stack:  [+]
Step 3:  5           → output: [3, 5]
Step 4:  *           → * has higher precedence than +, push
                       stack:  [+, *]
Step 5:  2           → output: [3, 5, 2]
Step 6:  end         → pop stack → output: [3, 5, 2, *, +]
         ↓
Postfix: 3 5 2 * +
Result:  3 + (5*2) = 13
```

---

## Exit Codes

| Code | Meaning |
|------|---------|
| `0` | Success |
| `1` | Syntax error |
| `2` | Division by zero |
| `3` | Logical / evaluation error |
