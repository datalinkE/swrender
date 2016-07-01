#include <cstdlib>
#include <functional>
#include "tgaimage.h"

const TGAColor white  = TGAColor(255, 255, 255, 255);
const TGAColor red    = TGAColor(255, 0,   0,   255);
const TGAColor green  = TGAColor(0,   255, 0,   255);
const TGAColor blue   = TGAColor(0,   0,   255, 255);
const TGAColor cyan   = TGAColor(0, 255,   255, 100);

struct Point
{
    double x;
    double y;
    TGAColor c;
};

struct Line
{
    double a;
    double b;
    int Cy;
    int y(int x) { return  (Cy ==0) ? (a * x + b) : Cy;}
};

class Draw
{
public:
    Draw(TGAImage* image);

    static void segment(int x0, int y0,
                     const TGAColor &c0,
                     int x1, int y1,
                     const TGAColor &c1,
                     std::function<bool (int, int, TGAColor)> callback);

    void segment(Point p0, Point p1);
    void triangle(Point p0, Point p1, Point p2);

    Line getLine(Point p0, Point p1);

    void done(const char* output_file);

private:
    TGAImage* mImage;
};
