#include <iostream>
using namespace std;
class A {
public:
    void show()
    {
        cout << "hello world" << endl;
    };
};
class B : public A {
public:
    void show()
    {
        A::show();
        cout << "hello B" << endl;
    }
};
class C : public A {
public:
    void show()
    {
        A::show();
        cout << "hello C" << endl;
    }
};
class D : public B, public C {
public:
    void show()
    {
        B::show();
        cout << "hello D" << endl;
    }
};
int main()
{
    D d = D();
    d.show();
    return 0;
}