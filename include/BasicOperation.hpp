#ifndef BasicOperation_hpp
#define BasicOperation_hpp
#include "EditOperationInterface.hpp"
#include <functional>
using std::function;

/**
 * This class aim to construct operation
 * with lambda expression.
 */
class BasicOperation: public EditOperationInterface {
    private:
        function<void()> executeFun;
        function<void()> undoFun;
    public:
        BasicOperation(const function<void()> & execute, const function<void()> & undo);
        void execute();
        void undo();
};

#endif
