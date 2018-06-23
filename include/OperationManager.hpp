#ifndef OperationManager_hpp
#define OperationManager_hpp
#include "EditOperationInterface.hpp"
#include <stack>
#include <memory>

using std::stack;
using std::shared_ptr;

class OperationManager {
    private:
        /**
         * Too hard to name it!!
         */
        stack<shared_ptr<EditOperationInterface>> history;
        stack<shared_ptr<EditOperationInterface>> undo_history;
    public:
        OperationManager() {}
        void executeOp(shared_ptr<EditOperationInterface>);

        /**
         * If there is nothing to undo, nothing happen
         */
        void undo();

        /**
         * This method undo undo.
         */
        void cancle_undo();
};

#endif /* OperationManager_hpp */