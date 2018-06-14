# OperationManager 文档

by 吴博文

wubw6@mail2.sysu.edu.cn 

版本号：1.0

## 简介

鉴于需求中需要实现撤销这样的需求，所以将每个Operation抽象出来就是十分必要的。

这是我关于实现撤销功能一点设想



## EditOperationInterface

首先我构造了这样一个interface，每种operation都应该继承他并实现`execute`和`undo`方法

```cpp
class EditOperationInterface {
    public:
        virtual void execute() = 0;
        virtual void undo() = 0;
};
```

## OperationManager

使用`shared_ptr`管理operation，operation的执行与撤销通过这个类实现

使用两个`stack`维护Opeartion:

```Cpp
stack<shared_ptr<EditOperationInterface>> history;
stack<shared_ptr<EditOperationInterface>> undo_history;
```

* `history`存放执行过的operation
* `undo_history`存放被撤销的operation，这个stack的作用是撤销 “撤销”（不知道怎么表达这个东西）



### executeOp(EditOperationInterface*)

```cpp

void OperationManager::executeOp(EditOperationInterface* new_op) {
    new_op->execute();
    ops.push(shard_ptr<EditOperationInterface>(new_op));
    // init undo_history
    undo_history = stack<shared_ptr<EditOperationInterface>>();
}

```

当有新的operation加入时，`undo_history`被清空

### undo()

```cpp
void OperationManager::undo() {
    if (!history.empty()) {
        auto to_undo = history.top();
        history.pop();
        to_undo->undo();
        undo_history.push(to_undo);
    }
}
```

undo `history`栈顶的operation，将其压至`undo_history`



### cancle_undo()

```cpp
void OperationManager::cancle_undo() {
    if (!undo_history.empty()) {
        auto undo_to_cancle = undo_history.top();
        undo_history.pop();
        undo_to_cancle.execute();
        history.push(undo_to_cancle);
    }
}
```



## BasicOperation

`EditOperationInterface`的一个简单实现，使用lambda表达式定义execute和undo的行为

不过我还没试过我写的对不对

### 构造函数

```cpp
BasicOperation(const function<void()> & execute, const function<void()> & undo);
```

使用方法如下（想象中）：

```cpp
operationManager.execute(new BasicOperation(
	[&](){
        // execute edit
	},
	[&](){
        // undo edit
	}))
```



## GroupEditOperation

一组Operation组成的一个Operation

我觉得lambda表达式比较好

### 构造函数

```Cpp
GroupEditOperation(const vector<shared_ptr<EditOperationInterface>> & ops);
```

执行顺序：第0个到第n-1个

