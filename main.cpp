#include <cstdlib>
#include "tgaimage.h"

//#define WITH_GOOGLE_PERFTOOLS

#ifdef WITH_GOOGLE_PERFTOOLS

#include <gperftools/profiler.h>
#define START_PROF(arg) ProfilerStart(arg);
#define STOP_PROF() ProfilerStop();

#else

#define START_PROF(arg) ;
#define STOP_PROF() ;

#endif

const TGAColor white  = TGAColor(255, 255, 255, 255);
const TGAColor red    = TGAColor(255, 0,   0,   255);
const TGAColor green  = TGAColor(0,   255, 0,   255);
const TGAColor blue   = TGAColor(0,   0,   255, 255);
const TGAColor cyan   = TGAColor(255, 0,   255, 255);


class Draw
{
public:
    Draw(TGAImage* image);

    static void line(TGAImage* image,
                     int x0, int y0,
                     int x1, int y1,
                     const TGAColor* color);

    void line(int x0, int y0, int x1, int y1, const TGAColor* color);

private:
    TGAImage* mImage;
};

Draw::Draw(TGAImage* image)
    : mImage(image)
{}

void Draw::line(TGAImage* image,
                int x0, int y0,
                int x1, int y1,
                const TGAColor* color)
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
            image->set(y, x, *color);
        }
        else
        {
            image->set(x, y, *color);
        }
    }
}

void Draw::line(int x0, int y0, int x1, int y1, const TGAColor* color)
{
    Draw::line(mImage, x0, y0, x1, y1, color);
}

void test(TGAImage* image)
{
    START_PROF("./profiler.out");
    for (int i=0; i < 1000000; ++i)
    {
        Draw draw(image);
        draw.line(250, 100, 120, 380, &red);
        draw.line(10, 76, 371, 320, &green);
        draw.line(30, 250, 350, 135, &cyan);
    }
    STOP_PROF();
}

int main(int argc, char** argv)
{
    TGAImage image(400, 400, TGAImage::RGB);
    image.set(10, 10, white);

    test(&image);

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}
