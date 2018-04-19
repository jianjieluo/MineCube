# CraftManager 使用说明

by 吴博文

wubw6@mail2.sysu.edu.cn

版本号：1.0

## 简介

计划完成的任务：使用若干个小正方体组成一个大正方体，以达到可编辑方块的目的

## 方法说明

```cpp
CraftManager(GLfloat sizePerCube,
                    unsigned int numPerEdge,
                    unsigned int shaderID,
                    string modelMat4Name,
                    const vector<GLuint>& attriSize)
```

参数说明：

* sizePerCube: 每个小立方体的边长

* numPerEdge:对于大立方体，组成每条边的小立方体的个数。即一个大立方体由$numPerEdge^3$个小立方体组成

* shaderID: shader 的ID

* modelMat4Name:着色器用以将局部坐标转换至世界坐标的uniform矩阵名

* attriSize: 对于传给顶点着色器的属性，每个属性所对应的占用空间大小（计数单位为sizeof(GLfloat)). 需要注意的是，忽略每个点的位置信息所占用的空间。如：
   每个顶点的格式为：

  ```
  1.0f, -1.0f, -1.0f, 0.0f, 1.0f
  ```

  前三个代表顶点的坐标，后两个指某种需要传递给顶点着色器的属性，则传入attriSize应为：

  ```Cpp
  [2]
  ```

功能说明：

构造函数负责申请绑定VAO、VBO

----

```Cpp
void draw()
```

负责画立方体，在循环渲染中调用



---

```cpp
void setAttriArray(unsigned int offset, unsigned int size, const vector<GLfloat> & data)
```

用于设置每个顶点除了位置以外的属性

（当然，你想设置位置，我也没有验证23333）

参数说明：

* Offset:当前想设置的属性在顶点数组中的偏移值. 

* size：你想设置的属性的长度

* data：准备设置的数据，需要注意，这个vector的size应该为size * 36

* 如： 每个顶点的格式为：

  ```cpp
  1.0f, -1.0f, -1.0f, 0.0f, 1.0f
  ```

  我们想设置后两个属性，将其改为1.0f, 0.0f

  则传入的参数为:

  ```
  void setAttriArray(0, 2, [1.0f, 0.0f])
  ```

**这个方法还没弄好现在这种方式会给编程带来负担，我之后会改**



## 例程

```Cpp
// 初始化工作

// 构造一个Manager
CraftManager(GLfloat sizePerCube,
                    unsigned int numPerEdge,
                    unsigned int shaderID,
                    string modelMat4Name,
                    const vector<GLuint>& attriSize)
 
while() {
    // 循环渲染
    CraftManager.draw();
}

// 结束

```

