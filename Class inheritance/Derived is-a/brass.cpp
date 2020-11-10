#include "brass.h"
#include <iostream>
using std::cout;
using std::endl;
using std::string;

BrassPlus::BrassPlus(const string& s, long an, double bal, double ml, double r)
    : Brass(s, an, bal)
{
    maxLoan = ml;
    owesBank = 0.0;
    rate = r;
}

BrassPlus::BrassPlus(const Brass& ba, double ml, double r)
    : Brass(ba)
{
    maxLoan = ml;
    owesBank = 0.0;
    rate = r;
}
