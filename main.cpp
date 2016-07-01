#include <cstdlib>
#include <memory>
#include <cmath>
#include "model.h"
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
const char* DEFAULT_OBJ_IN = "./obj/african_head/african_head.obj";
const float PI = 3.14f;

void roundTest(Draw* draw) {
    for (float i = 0; i < (PI * 2); i+= ( PI / 6)) {
    float r = 0.9f;
    draw->triangle(
        Point{ Vec3f { 0 , 0, 0}, red},
        Point{ Vec3f { r * (float)cos( i ),  r * (float)sin( i ), 0}, red},
        Point{ Vec3f { r * (float)cos( i + 0.1f),  r * (float)sin( i + 0.1f), 0}, red});
    }
}

void test(Draw* draw) {
    draw->segment(Point{ Vec3f {-1, 0, 0}, red}, Point { Vec3f {1, 0, 0}, cyan});
    draw->segment(Point{ Vec3f {0, -1, 0}, green}, Point{ Vec3f {0, 1, 0}, green});
    draw->triangle(Point{ Vec3f {0.66, 1, 0}, red}, Point{ Vec3f {0.5, -0.2, 0}, red}, Point{Vec3f {-0.8, 0.1, 0}, red});
    roundTest(draw);
}

void payload(Draw* draw, Model* model) {
    for (int n=0; n < model->nfaces(); ++n) {
    std::vector<int> vertIds = model->face(n);

    auto p0 = Point { model->vert(vertIds[0]), red };
    auto p1 = Point { model->vert(vertIds[1]), red };
    auto p2 = Point { model->vert(vertIds[2]), red };

    std::cout << p0.v.x << " " << p0.v.y << " "
        << p0.v.x << " " << p0.v.y << " "
        << p0.v.x << " " << p0.v.y << std::endl;

    draw->triangle(p0, p1, p2);

    draw->segment(p0, p1);
    draw->segment(p1, p2);
    draw->segment(p2, p0);
    }
}

void test(Draw* draw, Model* model) {
    START_PROF(PROFILER_OUT);
    for (int i=0; i < 1000000; ++i) {
    payload(draw, model);
    }
    STOP_PROF();
}

void run(TGAImage* image, Model* model) {
    Draw draw(image);
#ifdef WITH_GOOGLE_PERFTOOLS
    test(&draw);
#else
    payload(&draw, model);
#endif
    draw.done(IMAGE_OUT);
}

int main(int argc, char** argv) {
    std::unique_ptr<Model> model;

    if (2==argc) {
        model.reset(new Model(argv[1]));
    } else {
        model.reset(new Model(DEFAULT_OBJ_IN));
    }

    TGAImage image(400, 400, TGAImage::RGB);
    image.set(10, 10, white);

    run(&image, model.get());

    return 0;
}
