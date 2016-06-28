#include <cstdlib>
#include <memory>
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

void payload(Draw* draw, Model* model)
{
	draw->line(Point{-1, 0, red}, Point {1, 0, red});
	draw->line(Point{0, -1, green}, Point{0, 1, green});
	draw->triangle(Point{0.66 ,1, white}, Point{0.5, 0.2, white}, Point{-1, 0, white});

	for (int n=0; n < model->nfaces(); ++n)
	{
		std::vector<int> vertIds = model->face(n);
		auto p0 = Point { model->vert(vertIds[0]).x, model->vert(vertIds[0]).y, white };
		auto p1 = Point { model->vert(vertIds[1]).x, model->vert(vertIds[1]).y, white };
		auto p2 = Point { model->vert(vertIds[2]).x, model->vert(vertIds[2]).y, white };

		std::cout << p0.x << " " << p0.y << " "
			<< p0.x << " " << p0.y << " "
			<< p0.x << " " << p0.y << std::endl;

		draw->line(p0, p1);
		draw->line(p1, p2);
		draw->line(p2, p0);
	}
}

void test(Draw* draw, Model* model)
{
    START_PROF(PROFILER_OUT);
    for (int i=0; i < 1000000; ++i)
    {
        payload(draw, model);
    }
    STOP_PROF();
}

void run(TGAImage* image, Model* model)
{
    Draw draw(image);
#ifdef WITH_GOOGLE_PERFTOOLS
    test(&draw);
#else
    payload(&draw, model);
#endif
    draw.done(IMAGE_OUT);
}

int main(int argc, char** argv)
{
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
