- 当类声明中包含纯虚函数时，则不能创建该类的对象。

### 纯虚函数

- 在基类中仅仅给出声明，不对虚函数实现定义，而是在派生类中实现。这个虚函数称为纯虚函数。普通函数如果仅仅给出它的声明而没有实现它的函数体，这是编译不过的。纯虚函数没有函数体。
- 纯虚函数需要在声明之后加个=0。

### 抽象类

- 含有纯虚函数的类叫抽象类。
- 抽象类只能作为派生类的基类，不能定义对象，但可以定义指针。
- 在派生类实现该纯虚函数后，定义抽象类对象的指针，并指向或引用子类对象。
- 在定义纯虚函数时，不能定义虚函数的实现部分。
- 在没有重新定义这种纯虚函数之前，是不能调用这种函数的。