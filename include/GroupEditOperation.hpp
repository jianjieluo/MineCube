#ifndef GroupEditOperation_hpp
#define GroupEditOperation_hpp
#include "EditOperationInterface.hpp"
#include <vector>
#include <memory>

using std::vector;
using std::shared_ptr;

class GroupEditOperation: public EditOperationInterface {
    private:
        vector<shared_ptr<EditOperationInterface>> ops;
    public:
        GroupEditOperation(const vector<shared_ptr<EditOperationInterface>> & ops);

        // override
        void excute();

        // override
        void undo();
};
#endif