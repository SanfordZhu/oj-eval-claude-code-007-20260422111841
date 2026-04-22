/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface, which is an excellent model for
 * the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include <string>
#include "evalstate.hpp"
#include "exp.hpp"
#include "Utils/tokenScanner.hpp"
#include "program.hpp"
#include "parser.hpp"
#include "Utils/error.hpp"

class Program;

class Statement {
public:
    Statement();
    virtual ~Statement();
    virtual void execute(EvalState &state, Program &program) = 0;
};

class RemStatement : public Statement {
public:
    RemStatement(TokenScanner &scanner);
    void execute(EvalState &state, Program &program) override;
};

class LetStatement : public Statement {
public:
    LetStatement(TokenScanner &scanner);
    ~LetStatement();
    void execute(EvalState &state, Program &program) override;
private:
    Expression *exp = nullptr;
};

class PrintStatement : public Statement {
public:
    PrintStatement(TokenScanner &scanner);
    ~PrintStatement();
    void execute(EvalState &state, Program &program) override;
private:
    Expression *exp = nullptr;
};

class InputStatement : public Statement {
public:
    InputStatement(TokenScanner &scanner);
    void execute(EvalState &state, Program &program) override;
private:
    std::string var;
};

class EndStatement : public Statement {
public:
    EndStatement(TokenScanner &scanner);
    void execute(EvalState &state, Program &program) override;
};

class GotoStatement : public Statement {
public:
    GotoStatement(TokenScanner &scanner);
    void execute(EvalState &state, Program &program) override;
private:
    int target;
};

class IfStatement : public Statement {
public:
    IfStatement(TokenScanner &scanner);
    ~IfStatement();
    void execute(EvalState &state, Program &program) override;
private:
    Expression *lhs = nullptr;
    std::string op;
    Expression *rhs = nullptr;
    int target;
};

#endif
