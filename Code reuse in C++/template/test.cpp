#include <iostream>

template <typename T>
class Test {
public:
    void show();
};

template <typename T>
void Test<T>::show()
{
    std::cout << "Hello!" << std::endl;
}
int main()
{
    Test<int> t = Test<int>();
    t.show();
    return 0;
}