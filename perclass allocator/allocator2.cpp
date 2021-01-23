#include <cstddef>
#include <iostream>

using namespace std;
class Airplane
{
private:
    struct AirplaneRep
    {
        unsigned long miles;
        char type;
    };

private:
    union
    {
        AirplaneRep rep;
        Airplane *next;
    };

public:
    unsigned long getMiles() const
    {
        return rep.miles;
    }
    char getType() const
    {
        return rep.type;
    }
    void set(unsigned long m, char t)
    {
        rep.miles = m;
        rep.type = t;
    }

public:
    static void *operator new(size_t size);
    static void operator delete(void *deadObject, size_t size);

private:
    static const int BLOCK_SIZE;
    static Airplane *headOfFreeList;
};
Airplane *Airplane::headOfFreeList;
const int Airplane::BLOCK_SIZE = 512;

void *Airplane::operator new(size_t size)
{
    if (size != sizeof(Airplane))
        return ::operator new(size);
    Airplane *p = headOfFreeList;
    if (p)
        headOfFreeList = p->next;
    else
    {
        Airplane *newBlock = static_cast<Airplane *>(::operator new(BLOCK_SIZE * sizeof(Airplane)));

        for (int i = 1; i < BLOCK_SIZE - 1; ++i)
        {
            // 新块的next指针指向下一块
            newBlock[i].next = &newBlock[i + 1];
        }
        // 结尾置为0
        newBlock[BLOCK_SIZE - 1].next = 0;
        p = newBlock;
        headOfFreeList = &newBlock[1];
    }
    return p;
}
// 发生错误时使用
void Airplane::operator delete(void *deadObject, size_t size)
{
    if (deadObject == 0)
        return;
    if (size != sizeof(Airplane))
    {
        ::operator delete(deadObject);
        return;
    }
    Airplane *carcass = static_cast<Airplane *>(deadObject);
    carcass->next = headOfFreeList;
    headOfFreeList = carcass;
}
int main(int argc, char const *argv[])
{
    cout << sizeof(Airplane) << endl;
    size_t const N = 100;
    Airplane *p[N];

    for (int i = 0; i < N; ++i)
    {
        p[i] = new Airplane;
    }
    p[1]->set(1000, 'A');
    p[5]->set(1000, 'B');
    p[9]->set(3000, 'C');
    for (int i = 0; i < 10; ++i)
        cout << p[i] << endl;

    for (int i = 0; i < N; ++i)
        delete p[i];
    return 0;
}
