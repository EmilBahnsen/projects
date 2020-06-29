//
//  main.c
//  asmManbrt
//
//  Created by Emil Bahnsen on 06-08-15.
//  Copyright © 2015 Emil Bahnsen. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

typedef struct _Point {
	double x,y;
} Point;

typedef struct _Size {
	double height, width;
} Size;

typedef struct _Rect {
	Point origin;
	Size size;
} Rect;

//int mandelbrotf(Rect* rect, Size* outSize, const char* outFile);
int mandelbrotb(Rect* rect, Size* outSize, const int8_t* outBuf);

int main(int argc, const char * argv[]) {

	Rect rect;
//	rect.origin = (Point){0.3750001200618655f,-0.2166393884377127f};
//	rect.size = (Size){0.02f,0.02f};
//	rect.origin = (Point){-0.13856524454488f,-0.64935990748190f};
//	rect.size = (Size){.00000000045f,.00000000045f};
	rect.origin = (Point){-1.2333819241982507f,0.11961389013081877f};
	rect.size = (Size){.002f,.002f};
//	rect.origin = (Point){0.0f,1.075f};
//	rect.size = (Size){0.03f,0.01f};
//	rect.origin = (Point){-2.0,-2.0};
//	rect.size = (Size){4.0,4.0};



	Size size;
	size.height = 600;
	size.width = 600;
//	size.height = 760;
//	size.width = 760;
//	size.height = 736;
//	size.width = 414;
//	rect.origin = (Point){-0.1592,-1.0317};
//	rect.size = (Size){size.height/2e3, size.width/2e3};

	remove("/Users/EmilB/Dropbox/Projects/asmManbrt/out.ppm");
	system("osascript -e 'quit app \"ToyViewer\"'"); // Close the image

	int bufSize = size.height * size.width;
	int8_t* buf = malloc(bufSize * 3);

	clock_t start = clock(), diff;
	mandelbrotb(&rect, &size, buf);

	diff = clock() - start;
	double msec = diff * 1000 / CLOCKS_PER_SEC;
	printf("Time taken %i seconds %d milliseconds\n", (int)(msec/1000), (int)msec%1000);
	double pixelsum = size.height * size.width;
	printf("Processed pixels: %i\n", (int)pixelsum);
	msec /= pixelsum;
	double nanosec = msec * 1e6;
	double nsPerCycle = 1/2.66;
	printf("Time per pixel: %f ns, i.e. approx. %f cycles.\n", nanosec, nanosec / nsPerCycle);

	int fd = open("/Users/EmilB/Dropbox/Projects/asmManbrt/out.ppm", O_WRONLY | O_APPEND | O_CREAT, 0b110100100);

	// Write header
	dprintf(fd, "P5\n%i %i\n255\n", (int)size.width, (int)size.height);

	write(fd, buf, bufSize);
	close(fd);

	free(buf);

	system("open /Users/EmilB/Dropbox/Projects/asmManbrt/out.ppm"); // Open the image

	return 0;
}

void measure(Rect* rect, Size* size, int itter) {
	clock_t start = clock(), diff;
	for (size_t i = 0; i < itter; ++i) {
		//mandelbrotf(rect, size, "");
	}
	diff = clock() - start;

	double msec = diff * 1000 / CLOCKS_PER_SEC;
	printf("Time taken %i seconds %d milliseconds\n", (int)(msec/1000), (int)msec%1000);
	double pixelsum = size->height * size->width * itter;
	printf("Processed pixels: %i\n", (int)pixelsum);
	msec /= pixelsum;
	double nanosec = msec * 1e6;
	double nsPerCycle = 1/2.66;
	printf("Time per pixel: %.f ns, i.e. approx. %.f cycles.\n", nanosec, nanosec / nsPerCycle);

}
