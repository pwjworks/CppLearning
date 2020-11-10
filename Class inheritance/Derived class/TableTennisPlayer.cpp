#include "TableTennisPlayer.h"
#include <iostream>
using namespace std;

// NOTE 成员初始化列表
TableTennisPlayer::TableTennisPlayer(const string& name, bool ht)
    : name(name)
    , hasTable(ht)
{
}

void TableTennisPlayer::Name() const
{
    cout << name << endl;
}