#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "draw.h"

const Vec3f LIGHT_DIR {0, 0, -1};

Draw::Draw(TGAImage* image)
    : mImage(image) {}

void Draw::screenSegment(int x0, int y0,
                const TGAColor &c0,
                int x1, int y1,
                const TGAColor & /*c1*/,
                std::function<bool (int, int, TGAColor )> callback) {
    bool transposed = std::abs(y0 - y1) > std::abs(x0 - x1);

    if (transposed) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dX = x1 - x0;
    int dY = y1 - y0;

    float stepY = (float) dY / dX;

    for (int i = 0; i < dX; ++i) {
        int x = x0 + i;
        int y = y0 + stepY * i;

        if (transposed) {
            callback(y, x, c0);
        }
        else {
            callback(x, y, c0);
        }
    }
}

void Draw::segment(Point p0, Point p1) {
    int halfw = mImage->get_width() / 2;
    int halfh = mImage->get_height() / 2;

    auto callback = std::bind(&TGAImage::set, mImage, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    Draw::screenSegment(halfw * (1 + p0.v.x), halfh * (1 + p0.v.y), p0.c, halfw * (1 + p1.v.x), halfh * (1 + p1.v.y), p1.c, callback);
}

Line Draw::getScreenLine(Point p0, Point p1) {
    int halfw = mImage->get_width() / 2;
    int halfh = mImage->get_height() / 2;

    float len = p1.v.x - p0.v.x;
    float a = 0;
    float b = 0;
    int Cy = 0;
    if ( len != 0.0 ) {
        a =  (p1.v.y - p0.v.y) * halfh / (len * halfw);
        b = halfh * (1 + p1.v.y) - halfw * (1 + p1.v.x) * a;
        Cy = p0.v.y;
    }
    std::cout << "a " << a << " b " << b << " len " << len << std::endl;
    return Line {a , b, Cy};
}


void Draw::triangle(Point p0, Point p1, Point p2) {
    std::vector<Point> points {p0, p1, p2};
    std::sort(points.begin(), points.end(), [](const Point &a, const Point &b) { return a.v.x < b.v.x; } );
    for(auto& p : points) {
        std::cout << "" << p.v.x << " " << p.v.y << std::endl;
    }

    Line AB = getScreenLine(points[0], points[2]);

    Vec3f n = (p2.v - p0.v) ^ (p1.v - p0.v); // cross product
    n.normalize();

    auto callback = [&](int x, int y, TGAColor /*c*/ ) {
        int segmentY = AB.y(x);
        int start = std::min<int>(y, segmentY);
        int stop = std::max<int>(y, segmentY);

        if (start > stop) std::swap(start, stop);

        std::cout << "x " << x << " y " << y << " start " << start << " stop " << stop << std::endl;
        for (int j = start; j < stop; j++) {
            float intensity = n * LIGHT_DIR; // dot product
            if (intensity > 0) {
                this->mImage->set(x, j, TGAColor(intensity*255, intensity*255, intensity*255, 255));
            }
        }

        return true;
    };

    auto s0 = fromWorld(points[0].v);
    auto s1 = fromWorld(points[1].v);
    auto s2 = fromWorld(points[2].v);

    Draw::screenSegment(s0.x, s0.y, points[0].c, s1.x, s1.y, points[1].c, callback);
    Draw::screenSegment(s1.x, s1.y, points[1].c, s2.x, s2.y, points[2].c, callback);
}

Vec2i Draw::fromWorld(Vec3f worldP) {
    int halfw = mImage->get_width() / 2;
    int halfh = mImage->get_height() / 2;
    return Vec2i(halfw * (1 + worldP.x), halfh * (1 + worldP.y));
}

void Draw::done(const char* output_file) {
    mImage->flip_vertically();
    mImage->write_tga_file(output_file);
}
