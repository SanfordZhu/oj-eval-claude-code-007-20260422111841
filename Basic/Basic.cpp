/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"

void processLine(std::string line, Program &program, EvalState &state);
static void runProgram(Program &program, EvalState &state);

int main() {
    EvalState state;
    Program program;
    while (true) {
        try {
            std::string input;
            if (!std::getline(std::cin, input)) break;
            if (input.empty()) continue;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }
    return 0;
}

void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);

    std::string first = scanner.nextToken();
    if (first == "") return;
    TokenType ttype = scanner.getTokenType(first);
    if (ttype == NUMBER) {
        int lineNumber = stringToInteger(first);
        std::string rest;
        if (scanner.hasMoreTokens()) {
            std::string rem = line.substr(line.find(first) + first.size());
            if (rem.size() && rem[0] == ' ') rem.erase(0,1);
            rest = rem;
        } else {
            program.removeSourceLine(lineNumber);
            return;
        }
        program.addSourceLine(lineNumber, line);
    } else {
        std::string cmd = first;
        if (cmd == "REM") {
            // ignore
        } else if (cmd == "LET") {
            Statement *stmt = new LetStatement(scanner);
            stmt->execute(state, program);
            delete stmt;
        } else if (cmd == "PRINT") {
            Statement *stmt = new PrintStatement(scanner);
            stmt->execute(state, program);
            delete stmt;
        } else if (cmd == "INPUT") {
            Statement *stmt = new InputStatement(scanner);
            stmt->execute(state, program);
            delete stmt;
        } else if (cmd == "END") {
            // immediate END does nothing
        } else if (cmd == "GOTO") {
            error("SYNTAX ERROR");
        } else if (cmd == "IF") {
            error("SYNTAX ERROR");
        } else if (cmd == "RUN") {
            runProgram(program, state);
        } else if (cmd == "LIST") {
            for (auto ln = program.getFirstLineNumber(); ln != -1; ln = program.getNextLineNumber(ln)) {
                std::cout << program.getSourceLine(ln) << std::endl;
            }
        } else if (cmd == "CLEAR") {
            program.clear();
            state.Clear();
        } else if (cmd == "QUIT") {
            exit(0);
        } else {
            error("SYNTAX ERROR");
        }
    }
}

static void runProgram(Program &program, EvalState &state) {
    program.setCurrentLine(program.getFirstLineNumber());
    while (true) {
        int ln = program.getCurrentLine();
        if (ln == -1) break;
        std::string src = program.getSourceLine(ln);
        TokenScanner sc;
        sc.ignoreWhitespace();
        sc.scanNumbers();
        sc.setInput(src);
        std::string lnTok = sc.nextToken();
        std::string cmd = sc.nextToken();
        Statement *stmt = nullptr;
        if (cmd == "REM") {
            // do nothing
        } else if (cmd == "LET") {
            stmt = new LetStatement(sc);
        } else if (cmd == "PRINT") {
            stmt = new PrintStatement(sc);
        } else if (cmd == "INPUT") {
            stmt = new InputStatement(sc);
        } else if (cmd == "END") {
            stmt = new EndStatement(sc);
        } else if (cmd == "GOTO") {
            stmt = new GotoStatement(sc);
        } else if (cmd == "IF") {
            stmt = new IfStatement(sc);
        } else {
            error("SYNTAX ERROR");
        }
        if (stmt) {
            stmt->execute(state, program);
            delete stmt;
        }
        if (program.getCurrentLine() == ln) {
            int next = program.getNextLineNumber(ln);
            program.setCurrentLine(next);
        }
    }
}

