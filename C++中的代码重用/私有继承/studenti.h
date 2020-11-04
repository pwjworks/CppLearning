#ifndef STUDENTC_H_
#define STUDENTC_H_

#include <iostream>
#include <string>
#include <valarray>

class Student : private std::string, private std::valarray<double> {
private:
    typedef std::valarray<double> ArrayDb;
    std::ostream& arr_out(std::ostream& os) const;

public:
    Student()
        : std::string("Null Student")
        , ArrayDb()
    {
    }
    explicit Student(const std::string& s)
        : std::string(s)
        , ArrayDb()
    {
    }
    explicit Student(int n)
        : std::string("Nully")
        , ArrayDb(n)
    {
    }
    Student(const std::string& s, int n)
        : std::string(s)
        , ArrayDb(n)
    {
    }
    Student(const std::string& s, const ArrayDb& a)
        : std::string(s)
        , ArrayDb(a)
    {
    }
    Student(const char* str, const double* pd, int n)
        : std::string(str)
        , ArrayDb(pd, n)
    {
    }
    ~Student() { }
    double Average() const;
    double& operator[](int n);
    double operator[](int n) const;
    const std::string& Name() const;
};

#endif // !STUDENTC_H_
