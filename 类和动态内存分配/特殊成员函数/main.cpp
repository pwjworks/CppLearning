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
    // ! 复制构造函数
    Test(const Test& t)
    {
        cout << "copied!" << endl;
        num++;
        data = t.data;
    }
    ~Test()
    {
        --num;
        cout << data << " deleted, " << num << " remain." << endl;
    }
    friend std::ostream& operator<<(std::ostream& os, const Test& t)
    {
        os << t.data;
        return os;
    };
};
// NOTE: 静态数据成员在类声名中声名，在包含方法的文件中初始化。
int Test::num = 0;

int main()
{
    Test a = Test(1);
    cout << a << endl;
    Test b = a;
    cout << b << endl;
    return 0;
}