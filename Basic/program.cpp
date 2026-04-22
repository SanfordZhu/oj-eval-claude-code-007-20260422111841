/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"
#include "Utils/error.hpp"

Program::Program() = default;

Program::~Program() {
    clear();
}

void Program::clear() {
    for (auto &p : parsed) delete p.second;
    parsed.clear();
    lines.clear();
    currentLine = -1;
}

void Program::addSourceLine(int lineNumber, const std::string &line) {
    if (parsed.count(lineNumber)) {
        delete parsed[lineNumber];
        parsed.erase(lineNumber);
    }
    lines[lineNumber] = line;
}

void Program::removeSourceLine(int lineNumber) {
    if (parsed.count(lineNumber)) {
        delete parsed[lineNumber];
        parsed.erase(lineNumber);
    }
    lines.erase(lineNumber);
}

std::string Program::getSourceLine(int lineNumber) {
    auto it = lines.find(lineNumber);
    if (it == lines.end()) return "";
    return it->second;
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    if (!lines.count(lineNumber)) error("LINE NUMBER ERROR");
    if (parsed.count(lineNumber)) delete parsed[lineNumber];
    parsed[lineNumber] = stmt;
}

Statement *Program::getParsedStatement(int lineNumber) {
    auto it = parsed.find(lineNumber);
    if (it == parsed.end()) return nullptr;
    return it->second;
}

int Program::getFirstLineNumber() {
    if (lines.empty()) return -1;
    return lines.begin()->first;
}

int Program::getNextLineNumber(int lineNumber) {
    auto it = lines.upper_bound(lineNumber);
    if (it == lines.end()) return -1;
    return it->first;
}

bool Program::hasLine(int lineNumber) {
    return lines.count(lineNumber);
}

void Program::setCurrentLine(int lineNumber) {
    currentLine = lineNumber;
}

int Program::getCurrentLine() const {
    return currentLine;
}
