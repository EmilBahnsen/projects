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
	 rect.origin = (Point){-2.0f,-2.0f};
	 rect.size = (Size){4.0f,4.0f};


	Size size;
	size.height = 1200*3;
	size.width = 1200*3;

	remove("/Users/EmilB/Dropbox/Projects/asmManbrt/out.ppm");

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

	return 0;
}
