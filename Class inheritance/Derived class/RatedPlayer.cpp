#include "TableTennisPlayer.cpp"

class RatedPlayer : public TableTennisPlayer {
private:
    unsigned int rating;

public:
    // NOTE 使用成员初始化列表
    RatedPlayer(unsigned int r = 0, const string& name = "none", const bool ht = false)
        : TableTennisPlayer(name, ht) {};
    RatedPlayer(unsigned int r, const TableTennisPlayer& tp);
    unsigned int Rating() const
    {
        return rating;
    }
    void ResetRating(unsigned int r)
    {
        rating = r;
    }
};
// NOTE 将调用基类的复制构造函数。这会初始化类成员。
RatedPlayer::RatedPlayer(unsigned int r, const TableTennisPlayer& tp)
    : TableTennisPlayer(tp)
{
    rating = r;
}
// 以上也可写成
// RatedPlayer::RatedPlayer(unsigned int r, const TableTennisPlayer& tp)
//     : TableTennisPlayer(tp)
//     , rating(r)
// {
// }
int main()
{
    TableTennisPlayer player1("Tom", false);
    RatedPlayer rplayer1(1140, "Jerry", true);
    rplayer1.Name();
    // NOTE 基类引用和指针可以分别引用和指向派生类对象。
    TableTennisPlayer& player2 = rplayer1;
    player2.Name();

    TableTennisPlayer& player3 = rplayer1;
    player3.Name();

    // NOTE 基类指针可用于派生类对象。在函数形参也是同样的效果。
    TableTennisPlayer player4(rplayer1);
    player4.Name();
    return 0;
}
