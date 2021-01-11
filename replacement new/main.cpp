#include <string>
#include <iostream>
using namespace std;

class Foo
{
public:
    int _id;
    long _data;
    string _str;

public:
    Foo() : _id(0)
    {
        cout << "default ctor. this=" << this << " id=" << _id << endl;
    }
    Foo(int id) : _id(id)
    {
        cout << "ctor.this=" << this << " id=" << _id << endl;
    }
    ~Foo()
    {
        cout << "dtor.this=" << this << " id=" << _id << endl;
    }
    static void *operator new(size_t size);
    static void *operator new(size_t size, void *start);
    static void *operator new(size_t size, long extra);
    static void operator delete(void *ptr);
    static void *operator new[](size_t size);
    static void operator delete[](void *pdead);
};
// 1
void *Foo::operator new(size_t size)
{
    Foo *p = (Foo *)malloc(size);
    cout << "operator new" << endl;
    return p;
}
// 2
void *operator new(size_t size, void *start)
{
    return start;
}
// 3
void *operator new(size_t size, long extra)
{
    return malloc(size + extra);
}
void Foo::operator delete(void *ptr)
{
    cout << "delete" << endl;
    free(ptr);
}
void *Foo::operator new[](size_t size)
{
    Foo *p = (Foo *)malloc(size);
    cout << "operator new[]" << endl;
    return p;
}
void Foo::operator delete[](void *pdead)
{
    cout << "operator delete[]" << endl;
    free(pdead);
}
int main()
{
    Foo *p = new Foo(7);
    delete p;
    Foo *pArray = new Foo[6];
    delete[] pArray;
}