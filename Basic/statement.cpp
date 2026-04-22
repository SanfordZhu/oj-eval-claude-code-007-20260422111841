/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"
#include <iostream>

Statement::Statement() = default;
Statement::~Statement() = default;

RemStatement::RemStatement(TokenScanner &scanner) {
    // consume the rest tokens as comment
    while (scanner.hasMoreTokens()) scanner.nextToken();
}
void RemStatement::execute(EvalState &state, Program &program) { (void)state; (void)program; }

LetStatement::LetStatement(TokenScanner &scanner) {
    scanner.addOperator("=");
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    exp = parseExp(scanner);
}
LetStatement::~LetStatement() { delete exp; }
void LetStatement::execute(EvalState &state, Program &program) {
    (void)program;
    (void)exp->eval(state);
}

PrintStatement::PrintStatement(TokenScanner &scanner) {
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    exp = parseExp(scanner);
}
PrintStatement::~PrintStatement() { delete exp; }
void PrintStatement::execute(EvalState &state, Program &program) {
    (void)program;
    std::cout << exp->eval(state) << std::endl;
}

InputStatement::InputStatement(TokenScanner &scanner) {
    scanner.ignoreWhitespace();
    std::string name = scanner.nextToken();
    TokenType type = scanner.getTokenType(name);
    if (type != WORD && type != NUMBER) error("SYNTAX ERROR");
    var = name;
}
void InputStatement::execute(EvalState &state, Program &program) {
    (void)program;
    std::string token;
    while (true) {
        std::cout << " ? ";
        if (!std::getline(std::cin, token)) error("INPUT ERROR");
        TokenScanner sc(token);
        sc.ignoreWhitespace();
        sc.scanNumbers();
        std::string t = sc.nextToken();
        if (t.size() == 0) continue;
        if (sc.getTokenType(t) == NUMBER) {
            int val = stringToInteger(t);
            state.setValue(var, val);
            break;
        } else {
            std::cout << "INVALID NUMBER" << std::endl;
        }
    }
}

EndStatement::EndStatement(TokenScanner &scanner) { (void)scanner; }
void EndStatement::execute(EvalState &state, Program &program) {
    (void)state;
    program.setCurrentLine(-1);
}

GotoStatement::GotoStatement(TokenScanner &scanner) {
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    std::string t = scanner.nextToken();
    if (scanner.getTokenType(t) != NUMBER) error("SYNTAX ERROR");
    target = stringToInteger(t);
}
void GotoStatement::execute(EvalState &state, Program &program) {
    (void)state;
    if (!program.hasLine(target)) error("LINE NUMBER ERROR");
    program.setCurrentLine(target);
}

IfStatement::IfStatement(TokenScanner &scanner) {
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    lhs = readE(scanner, 2);
    op = scanner.nextToken();
    if (op != "=" && op != "<" && op != ">") error("SYNTAX ERROR");
    rhs = readE(scanner, 2);
    std::string thenTok = scanner.nextToken();
    if (thenTok != "THEN") error("SYNTAX ERROR");
    std::string t = scanner.nextToken();
    if (scanner.getTokenType(t) != NUMBER) error("SYNTAX ERROR");
    target = stringToInteger(t);
}
IfStatement::~IfStatement() { delete lhs; delete rhs; }
void IfStatement::execute(EvalState &state, Program &program) {
    (void)program;
    int lv = lhs->eval(state), rv = rhs->eval(state);
    bool cond = false;
    if (op == "=") cond = (lv == rv);
    else if (op == "<") cond = (lv < rv);
    else if (op == ">") cond = (lv > rv);
    if (cond) {
        if (!program.hasLine(target)) error("LINE NUMBER ERROR");
        program.setCurrentLine(target);
    }
}
