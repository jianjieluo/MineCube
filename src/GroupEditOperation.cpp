//#include "GroupEditOperation.hpp"
//
//GroupEditOperation::GroupEditOperation(const vector<shared_ptr<EditOperationInterface>> & ops) {
//    this->ops = ops;
//}
//
//void GroupEditOperation::excute() {
//    for (auto begin = ops.begin(); begin != ops.end(); ++begin) {
//        (*begin)->execute();
//    }
//}
//
//void GroupEditOperation::undo() {
//    // undo should be in the reverse order of excute
//    for (auto rbegin = ops.rbegin(); rbegin != ops.rend(); ++rbegin) {
//        (*rbegin)->undo();
//    }
//}
