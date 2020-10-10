#include <iostream>
using namespace std;
class Test {
private:
    static int num;
    int data;

public:
    Test(int s)
    {
        data = s;
        num++;
    };
    ~Test()
    {
        num--;
        cout << "deleted, " << num << " remain.";
    }
    friend std::ostream& operator<<(std::ostream& os, const Test& t)
    {
        os << t.num;
        return os;
    };
};
// NOTE: 静态数据成员在类声名中声名，在包含方法的文件中初始化。
int Test::num = 0;

int main()
{
    Test a = Test(1);
    return 0;
}