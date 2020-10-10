#include <cstring>
#include <iostream>
using namespace std;
class Test {
private:
    static int num;
    int data;
    char* str; // 字符串指针
    int len;

public:
    Test(int s, char* st)
    {
        data = s;
        num++;
        len = strlen(st);
        str = new char[len + 1];
        strcpy(str, st);
    };
    // 复制构造函数
    Test(const Test& t)
    {
        cout << "copied!" << endl;
        num++;
        data = t.data;
        str = t.str;
        len = t.len;
    }
    // 析构函数
    ~Test()
    {
        --num;
        cout << data << " deleted, " << num << " remain." << endl;
    }
    //NOTE 友元函数，重载<<运算符
    friend std::ostream& operator<<(std::ostream& os, const Test& t)
    {
        os << t.data;
        return os;
    };
    // NOTE 将传入的Test对象进行深复制
    Test& operator=(const Test& t)
    {
        cout << "= operator triggered" << endl;
        if (this == &t) {
            return *this;
        }
        delete[] str;
        len = t.len;
        str = new char[len + 1];
        // string_s
        strcpy(str, t.str);
        return *this;
    }
    // NOTE 函数参数按值传递会导致复制
    void testparams(Test t)
    {
        cout << "testparams:" << t.data << endl;
    }
};
//NOTE 静态数据成员在类声名中声名，在包含方法的文件中初始化。
int Test::num = 0;

int main()
{
    Test a = Test(1, "Hello");
    cout << a << endl;
    Test b = a;
    cout << b << endl;
    a.testparams(b);
    b = a;
    return 0;
}