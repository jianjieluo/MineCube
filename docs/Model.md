# MineCube-Model层说明文档

by 吴博文

wubw6@mail2.sysu.edu.cn

版本号：1.0

## 1. 简介

本层次代码对应于MVC架构的M，负责管理数据、实现CURD以及数据持久化。在我们的应用中，需要管理的数据为`Cube`和相对应的属性。

## 1.1 各类之间关系如下:

![](http://on-img.com/chart_image/5b1b83e5e4b02e4b26fea96c.png?_=1528530148157)

**图中箭头代表have关系**

### 1.2 各类职责的简要说明

* GLBufferManager: 负责缓冲区及数据的CRUD操作
* Cube：单位方块，自带位置、颜色、法向量等属性
* CubeManager：管理一个`Cube`的集合，CRUD操作



## 2. GLBufferManager

* 每个`GLBufferManger`对象管理**独立**的一块缓冲区
* 为了编码方便以及避免由不恰当的使用导致的性能overhead，只能通过`GetInstance`方法获取一个实例的shared_ptr

---

### GLBufferManager::getNewInstance

```Cpp
static shared_ptr<GLBufferManager> getNewInstance(
				const vector<GLuint> & attrisize,
				const GLuint & numRecord);
```

获取一个指向新实例的智能指针

参数说明：

* attrisize: 对于每条记录拥有的属性长度列表
* numRecord：记录的条数

举例：

```Cpp
auto a = GLBufferManager::getNewInstance({3, 3}, 3)
```

则相当于执行以下代码：

```Cpp
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof (GLfloat), (void*)(0 * sizeof(GLfloat)));
glEnableVertexAttribArray(0);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
glEnableVertexAttribArray(1);
```

### GLBufferManager::setAttriArray

```cpp
void setAttriArray(GLuint offset, GLuint size, const vector<GLfloat> & data);
```

设置某一属性的所有值

参数说明：

* Offset: 对于每条记录的该属性的offset
* size：这条属性的size
* data：这个属性的值，size 应该为 $$size * numRecord $$

### GLBufferManager::bind

```Cpp
void bind();
```

在绘制前调用这个方法，绑定缓冲区

### GLBufferManager::unbind

```Cpp
void unbind();
```

绘制结束调用这个方法，解除缓冲区的绑定



## 3. Cube

* **不要尝试复制一个Cube**
* **请使用shared_ptr管理Cube**

### Cube::Cube

```Cpp
Cube(
            GLfloat size,
            GLuint shaderID,
            string mat4Name,
            vector<GLuint> attriSize
            );
```

参数说明：

* size：大小
* shaderID:渲染该Cube使用的shader的ID
* mat4Name：在shader中uniform Model矩阵的名字
* attrisize：除去位置、颜色、法向量的额外属性

总是使用以下的方法创建Cube：

```Cpp
auto a = shared_ptr<Cube>(new Cube(...))
```

### Cube::editColor

```Cpp
virtual void editColor(
            GLfloat r,
            GLfloat g,
            GLfloat b,
            unsigned int plane
        );
```

参数说明：

* (r, g, b): (0.0f, 0.0f, 0.0f) ~ (1.0, 1.0f, 1.0f)
* plane:指定正方体六个面之一

plane可以以下值：

```Cpp
static const unsigned int LEFT;
static const unsigned int RIGHT;
static const unsigned int UP;
static const unsigned int BOTTOM;
static const unsigned int FRONT;
static const unsigned int BACK;
```



## 4. CubeManager

### CubeManager::CubeManager

```Cpp
CubeManager(
            unsigned int width,
            unsigned int height,
            unsigned int depth,
            GLfloat sizePerCube
        );
```

分别为大方块的长度、宽度、深度，以及内部每个小方块的颜色

**需要注意的是，此时Manager并没有拥有方块。可以这么说，这相当于新建了一个用于放置方块的抽屉，但是抽屉里还没有方块**

```cpp
CubeManager(string model_path) = delete;
```

从外部模型文件导入模型

### CubeManager::getCube

```cpp
shared_ptr<Cube> getCube(
            unsigned int x,
            unsigned int y,
            unsigned int z
        );
```

获取指向所管理的一个方块的指针

### CubeManager::setCube

```Cpp
void setCube(
            unsigned int x,
            unsigned int y,
            unsigned int z,
            shared_ptr<Cube> newCube
        );
```

增加/替换方块

### CubeManager::deleteCube

```Cpp
void deleteCube(
            unsigned int x,
            unsigned int y,
            unsigned int z
        );
```

删除特定的方块

### CubeManager::draw

```cpp
void draw();
```

在渲染循环中调用此方法

### CubeManager::defalut_init_all

```cpp
void defalut_init_all(const GLuint & shaderID, const string & mat4Name);
```

为所有的位置初始化一个小方块。

**相当于在每个抽屉里放入一样的方块**



### CubeManager::setAllShaderId

```cpp
void setAllShaderId(const GLuint & shaderID);
```

> Change all shaderID of cubes managed by this CubeManager
>
>   This is not a cheap operation.
>
> Warning: Calling this function will change ALL CUBE shader!
>
> If you just modify part of cubes' shader, use Cube::setShaderId instead.



### CubeManager::rotateHorizontal

```Cpp
void rotateHorizontal(const GLfloat & offset);
```

水平方向旋转

### CubeManager::rotateVertical

```cpp
 void rotateVertical(const GLfloat & offset);
```

垂直方向旋转

### CubeManager::setRotateSensivity

```cpp
void setRotateSensivity(const GLfloat & rotateSensivitiy);
```

修改旋转时的灵敏度

### CubeManager::getModelMat4

```cpp
glm::mat4 getModelMat4(
            unsigned int x,
            unsigned int y,
            unsigned int z
        );
```

获取某个位置的Model矩阵