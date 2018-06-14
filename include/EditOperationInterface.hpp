#ifndef EditOperationInterface_hpp
#define EditOperationInterface_hpp

/**
 * Interface for operation on cube
 */
class EditOperationInterface {
    public:
        virtual void execute() = 0;
        virtual void undo() = 0;
};

#endif /* EditOperation_hpp */