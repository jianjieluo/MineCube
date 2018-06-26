#include "BasicOperation.hpp"

BasicOperation::BasicOperation(const function<void()> & execute,
                                const function<void()> & undo) {
    this->executeFun = execute;
    this->undoFun = undo;
}

void BasicOperation::execute() {
    this->executeFun();
}

void BasicOperation::undo() {
    this->undoFun();
}