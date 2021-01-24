#include <string>
#include <iostream>
using namespace std;

class myAllocator
{
private:
    struct obj
    {
        // embedded pointer
        obj *next;
    };

public:
    void *allocate(size_t);
    void deallocate(void *, size_t);

private:
    obj *freeStore = nullptr;
    const int CHUNK = 5;
};
void myAllocator::deallocate(void *p, size_t size)
{
    ((obj *)p)->next = freeStore;
    freeStore = (obj *)p;
}
void *myAllocator::allocate(size_t size)
{
    obj *p;
    if (!freeStore)
    {
        size_t chunk = CHUNK * size;
        freeStore = p = (obj *)malloc(chunk);

        for (int i = 0; i < (CHUNK - 1); ++i)
        {
            p->next = (obj *)((char *)p + size);
            p = p->next;
        }
        p->next = nullptr;
    }
    p = freeStore;
    freeStore = freeStore->next;
    return p;
}
class Foo
{
public:
    long L;
    string str;
    static myAllocator myAlloc;

public:
    Foo(long i) : L(i) {}
    static void *operator new(size_t size)
    {
        return myAlloc.allocate(size);
    }
    static void operator delete(void *plead, size_t size)
    {
        cout << "delete" << endl;
        return myAlloc.deallocate(plead, size);
    }
};
myAllocator Foo::myAlloc;

int main(int argc, char const *argv[])
{
    Foo *p[30];
    // cout << sizeof(Foo) << endl;
    for (int i = 0; i < 23; ++i)
    {
        p[i] = new Foo(i);
        cout << p[i] << ' ' << p[i]->L << endl;
    }
    for (int i = 0; i < 23; ++i)
    {
        delete p[i];
    }
    return 0;
}
