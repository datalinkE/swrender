#include <cstdlib>
#include "draw.h"

#ifdef WITH_GOOGLE_PERFTOOLS

#include <gperftools/profiler.h>
#define START_PROF(arg) ProfilerStart(arg);
#define STOP_PROF() ProfilerStop();

#else

#define START_PROF(arg) ;
#define STOP_PROF() ;

#endif

const char* PROFILER_OUT = "./profiler.out";
const char* IMAGE_OUT = "./output.tga";

void payload(Draw* draw)
{
        draw->line(Point{-1, 0, red}, Point {1, 0, red});
        draw->line(Point{0, -1, green}, Point{0, 1, green});
        draw->triangle(Point{0.66 ,1, white}, Point{0.5, 0.2, white}, Point{-1, 0, white});
}

void test(Draw* draw)
{
    START_PROF(PROFILER_OUT);
    for (int i=0; i < 1000000; ++i)
    {
        payload(draw);
    }
    STOP_PROF();
}

void run(TGAImage* image)
{
    Draw draw(image);
#ifdef WITH_GOOGLE_PERFTOOLS
    test(&draw);
#else
    payload(&draw);
#endif
    draw.done(IMAGE_OUT);
}

int main(int /*argc*/, char** /*argv*/)
{
    TGAImage image(400, 400, TGAImage::RGB);
    image.set(10, 10, white);

    run(&image);

    return 0;
}
