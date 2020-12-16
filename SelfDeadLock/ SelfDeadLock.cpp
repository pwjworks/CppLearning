#include <mutex>
using namespace std;

class Request
{
private:
    mutex mtx;

public:
    void process()
    {
        mtx.lock();
        print();
    }
    void print()
    {
        mtx.lock();
    }
};
int main()
{
    Request req;
    req.process();
}