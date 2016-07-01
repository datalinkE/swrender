#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "draw.h"

Draw::Draw(TGAImage* image)
    : mImage(image)
{}

void Draw::segment(int x0, int y0,
                const TGAColor &c0,
                int x1, int y1,
                const TGAColor & /*c1*/,
                std::function<bool (int, int, TGAColor )> callback)
{
    bool transposed = std::abs(y0 - y1) > std::abs(x0 - x1);

    if (transposed)
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dX = x1 - x0;
    int dY = y1 - y0;

    float stepY = (float) dY / dX;

    for (int i = 0; i < dX; ++i)
    {
        int x = x0 + i;
        int y = y0 + stepY * i;

        if (transposed)
        {
            callback(y, x, c0);
        }
        else
        {
            callback(x, y, c0);
        }
    }
}

void Draw::segment(Point p0, Point p1)
{
    int halfw = mImage->get_width() / 2;
    int halfh = mImage->get_height() / 2;

    auto callback = std::bind(&TGAImage::set, mImage, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    Draw::segment(halfw * (1 + p0.x), halfh * (1 + p0.y), p0.c, halfw * (1 + p1.x), halfh * (1 + p1.y), p1.c, callback);
}

Line Draw::getLine(Point p0, Point p1)
{
    int halfw = mImage->get_width() / 2;
    int halfh = mImage->get_height() / 2;

    float len = p1.x - p0.x;
    float a = 0;
    float b = 0;
    int Cy = 0;
    if ( len != 0.0 ) {
        a =  (p1.y - p0.y) * halfh / (len * halfw);
        b = halfh * (1 + p1.y) - halfw * (1 + p1.x) * a;
        Cy = p0.y;
    }
    std::cout << "a " << a << " b " << b << " len " << len << std::endl;
    return Line {a , b, Cy};
}

void Draw::triangle(Point p0, Point p1, Point p2)
{
    int halfw = mImage->get_width() / 2;
    int halfh = mImage->get_height() / 2;

    std::vector<Point> points {p0, p1, p2};
    std::sort(points.begin(), points.end(), [](const Point &a, const Point &b) { return a.x < b.x; } );
    for(auto& p : points)
    {
        std::cout << "" << p.x << " " << p.y << std::endl;
    }

    Line AB = getLine(points[0], points[2]);

    auto callback = [&](int x, int y, TGAColor /*c*/ )
    {
        int segmentY = AB.y(x);
        int start = std::min<int>(y, segmentY);
        int stop = std::max<int>(y, segmentY);

        if (start > stop) std::swap(start, stop);

        std::cout << "x " << x << " y " << y << " start " << start << " stop " << stop << std::endl;
        for (int j = start; j < stop; j++) {

            this->mImage->set(x, j, cyan);
        }

        return true;
    };

    Draw::segment(halfw * (1 + points[0].x), halfh * (1 + points[0].y), points[0].c, halfw * (1 + points[1].x), halfh * (1 + points[1].y), points[1].c, callback);
    Draw::segment(halfw * (1 + points[1].x), halfh * (1 + points[1].y), points[1].c, halfw * (1 + points[2].x), halfh * (1 + points[2].y), points[2].c, callback);
}

void Draw::done(const char* output_file)
{
    mImage->flip_vertically();
    mImage->write_tga_file(output_file);
}
