#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "draw.h"

Draw::Draw(TGAImage* image)
    : mImage(image)
{}

void Draw::line(int x0, int y0,
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

void Draw::line(Point p0, Point p1)
{
    int halfw = mImage->get_width() / 2;
    int halfh = mImage->get_height() / 2;

    auto callback = std::bind(&TGAImage::set, mImage, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    Draw::line(halfw * (1 + p0.x), halfh * (1 + p0.y), p0.c, halfw * (1 + p1.x), halfh * (1 + p1.y), p1.c, callback);
}

void Draw::triangle(Point p0, Point p1, Point p2)
{
    std::vector<Point> points {p0, p1, p2};
    std::sort(points.begin(), points.end(), [](const Point &a, const Point &b) { return a.y < b.y; } );
    int halfw = mImage->get_width() / 2;
    int halfh = mImage->get_height() / 2;

    float len = points[2].x - points[0].x;
    float a =  (points[2].y - points[0].y) / len;
    float b = (points[0].y * points[2].x - points[2].y * points[0].x) / len;

    auto callback = [&](int x, int y, TGAColor c)
    {
        float lineY = x * a + b;
        int start = y;
        int stop = lineY + halfh;

        if (start > stop) std::swap(start, stop);

        for (int j = start; j < stop; j++) {

            this->mImage->set(x, j, c);
        }
        return true;
    };

    Draw::line(halfw * (1 + points[0].x), halfh * (1 + points[0].y), cyan, halfw * (1 + points[1].x), halfh * (1 + points[1].y), cyan, callback);
    Draw::line(halfw * (1 + points[1].x), halfh * (1 + points[1].y), cyan, halfw * (1 + points[2].x), halfh * (1 + points[2].y), cyan, callback);

    Draw::line(p0, p1);
    Draw::line(p1, p2);
    Draw::line(p2, p0);
}

void Draw::done(const char* output_file)
{
    mImage->flip_vertically();
    mImage->write_tga_file(output_file);
}
