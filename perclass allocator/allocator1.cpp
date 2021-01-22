#include <cstddef>
#include <iostream>

using namespace std;

class Screen
{
public:
    Screen(int x) : i(x){};
    int get()
    {
        return i;
    }
    void *operator new(size_t size);
    void operator delete(void *ptr);

private:
    Screen *next;
    static Screen *freeStore;
    static const int screenChunk;
    int i;
};

Screen *Screen::freeStore = 0;
const int Screen::screenChunk = 24;

void *Screen::operator new(size_t size)
{
    Screen *p;
    if (!freeStore)
    {
        size_t chunk = screenChunk * size;
        freeStore = p = reinterpret_cast<Screen *>(new char[chunk]);
        for (; p != &freeStore[screenChunk - 1]; ++p)
        {
            p->next = p + 1;
        }
        p->next = 0;
    }
    p = freeStore;
    freeStore = freeStore->next;
    return p;
}

void Screen::operator delete(void *p)
{
    (static_cast<Screen *>(p))->next = freeStore;
    freeStore = static_cast<Screen *>(p);
}

int main(int argc, char const *argv[])
{
    size_t const N = 100;
    cout << sizeof(Screen) << endl;
    Screen *p[N];

    for (int i = 0; i < N; ++i)
    {
        p[i] = new Screen(i);
    }
    for (int i = 0; i < 10; ++i)
    {
        cout << p[i] << endl;
    }
    for (int i = 0; i < N; ++i)
    {
        delete p[i];
    }
    return 0;
}
