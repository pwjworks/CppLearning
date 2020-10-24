class BaseEllipse {
private:
    double x;
    double y;

public:
    BaseEllipse(double x0, double y0)
        : x(x0)
        , y(y0)
    {
    }
    virtual ~BaseEllipse() { }
    void Move(int nx, int ny)
    {
        x = nx;
        y = ny;
    }
    virtual double Area() const = 0;
};