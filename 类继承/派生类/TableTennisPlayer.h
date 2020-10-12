#include <iostream>
#include <string>

using namespace std;

class TableTennisPlayer {
private:
    string name;
    bool hasTable;

public:
    TableTennisPlayer(const string& name = "none", bool ht = false);
    void Name() const;
    bool HasTable() const
    {
        return hasTable;
    }
    void ResetTable(bool v)
    {
        hasTable = v;
    }
};