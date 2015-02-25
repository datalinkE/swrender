#include "tgaimage.h"

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
                     const TGAColor &color);

    void line(int x0, int y0, int x1, int y1, const TGAColor &color);

private:
    TGAImage* mImage;
};

Draw::Draw(TGAImage* image)
    : mImage(image)
{}

void Draw::line(TGAImage* image,
                int x0, int y0,
                int x1, int y1,
                const TGAColor &color)
{
    for (float d=0; d <1; d+=.001)
    {
        int x = x0 * d + x1 * (1 - d);
        int y = y0 * d + y1 * (1 - d);
        image->set(x, y, color);
    }

}

void Draw::line(int x0, int y0, int x1, int y1, const TGAColor &color)
{
    Draw::line(mImage, x0, y0, x1, y1, color);
}

int main(int argc, char** argv) {
    TGAImage image(400, 400, TGAImage::RGB);
    image.set(10, 10, white);

    Draw draw(&image);
    draw.line(100, 100, 100, 300, red);
    draw.line(100, 100, 300, 100, green);
    draw.line(100, 100, 200, 300, blue);
    draw.line(100, 100, 300, 200, cyan);

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}
