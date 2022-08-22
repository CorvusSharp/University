#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <getopt.h>


#define PNG_DEBUG 3
#include "png.h"

struct Configs{
    png_byte r_cur, g_cur, b_cur, a_cur, r_rep, g_rep, b_rep, a_rep, r, g, b, a, line_r, line_g, line_b, line_a;
    png_byte find_r, find_g, find_b, find_a;
    int len, x1, y1, x2, y2, y, x, H, W, S, thickness;
    int** position;
    int replaceColor, makeFrame, findAllRectangle;
};

struct Png{
    int width, height;
    png_byte color_type;
    png_byte bit_depth;

    png_structp png_ptr;
    png_infop info_ptr;
    int number_of_passes;
    png_bytep *row_pointers;
};

void readPngFile(char *file_name, struct Png *image) {
    int x,y;
    char header[8];    // 8 is the maximum size that can be checked

    /* open file and test for it being a png */
    FILE *fp = fopen(file_name, "rb");
    if (!fp){
        // Some error handling: file could not be opened
    }

    fread(header, 1, 8, fp);

    /* initialize stuff */
    image->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
                                            NULL, NULL);

    if (!image->png_ptr){
        // Some error handling: png_create_read_struct failed
    }

    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr){
        // Some error handling: png_create_info_struct failed
    }

    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during init_io
    }

    png_init_io(image->png_ptr, fp);
    png_set_sig_bytes(image->png_ptr, 8);

    png_read_info(image->png_ptr, image->info_ptr);

    image->width = png_get_image_width(image->png_ptr, image->info_ptr);
    image->height = png_get_image_height(image->png_ptr, image->info_ptr);
    image->color_type = png_get_color_type(image->png_ptr,
                                           image->info_ptr);
    image->bit_depth = png_get_bit_depth(image->png_ptr, image->info_ptr);

    image->number_of_passes = png_set_interlace_handling(image->png_ptr);
    png_read_update_info(image->png_ptr, image->info_ptr);

    /* read file */
    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during read_image
    }

    image->row_pointers = (png_bytep *) malloc(sizeof(png_bytep) *
                                               image->height);
    for (y = 0; y < image->height; y++)
        image->row_pointers[y] = (png_byte *)
                malloc(png_get_rowbytes(image->png_ptr, image->info_ptr));

    png_read_image(image->png_ptr, image->row_pointers);

    fclose(fp);
}

void writePngFile(char *file_name, struct Png *image) {
    int x,y;
    /* create file */
    FILE *fp = fopen(file_name, "wb");
    if (!fp){
        // Some error handling: file could not be opened
    }

    /* initialize stuff */
    image->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL,
                                             NULL, NULL);

    if (!image->png_ptr){
        // Some error handling: png_create_write_struct failed
    }

    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr){
        // Some error handling: png_create_info_struct failed
    }

    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during init_io
    }

    png_init_io(image->png_ptr, fp);


    /* write header */
    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during writing header
    }

    png_set_IHDR(image->png_ptr, image->info_ptr, image->width,
                 image->height,
                 image->bit_depth, image->color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(image->png_ptr, image->info_ptr);


    /* write bytes */
    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during writing bytes
    }

    png_write_image(image->png_ptr, image->row_pointers);


    /* end write */
    if (setjmp(png_jmpbuf(image->png_ptr))){
        // Some error handling: error during end of write
    }

    png_write_end(image->png_ptr, NULL);

    /* cleanup heap allocation */
    for (y = 0; y < image->height; y++)
        free(image->row_pointers[y]);
    free(image->row_pointers);

    fclose(fp);
}

void replaceColor(struct Png *image, png_byte r_cur, png_byte g_cur,
                  png_byte  b_cur, png_byte a_cur, png_byte r_rep, png_byte  g_rep, png_byte
                  b_rep, png_byte a_rep) {
    int x,y;
    if (png_get_color_type(image->png_ptr, image->info_ptr) ==
        PNG_COLOR_TYPE_RGB){
        // Some error handling: input file is PNG_COLOR_TYPE_RGB but must
        //be PNG_COLOR_TYPE_RGBA
    }

    if (png_get_color_type(image->png_ptr, image->info_ptr) !=
        PNG_COLOR_TYPE_RGBA){
        // Some error handling: color_type of input file must be
        //PNG_COLOR_TYPE_RGBA
    }

    for (y = 0; y < image->height; y++) {
        png_byte *row = image->row_pointers[y];
        for (x = 0; x < image->width; x++) {
            png_byte *ptr = &(row[x * 4]);
            if (ptr[0] == r_cur  && ptr[1] == g_cur  && ptr[2] ==
                                                        b_cur && ptr[3] ==
                                                                 a_cur){
                ptr[0] = r_rep;
                ptr[1] = g_rep;
                ptr[2] = b_rep;
                ptr[3] = a_rep;
            }
        }
    }
}

typedef struct Point {
    int x;
    int y;
    int index;
} Point;

Point* findPointsUpKoha(Point p1, Point p2, Point* arrOfPoints, int len){
    Point pp1, pp2, pp3;
    int d1X, d1Y, d2X, d2Y, d3X, d3Y, d4X, d4Y;
    float h;

    if (p1.y == p2.y){
        int j = (int)(((float)(p2.x - p1.x))/3.0);

        d1X = p1.x + j;
        d1Y = p1.y;

        d2X = p1.x + (int)(((float)(p2.x - p1.x))/2.0);
        h = (sqrt(3.0)/2.0) * (((float)(p2.x - p1.x))/3.0);
        d2Y = p1.y + (int)(h);

        d3X = p1.x + 2 * j;
        d3Y = p1.y;
    } else if (p1.y > p2.y){
        float k1 = (float)(p2.x - p1.x);
        float k2 = (float)(p1.y - p2.y);
        float g = (sqrt(k1 * k1 + k2 * k2))/3.0;

        d1X = p2.x - (int)(g);
        d1Y = p2.y + (int)(g * sqrt(3.0));

        float n = (sqrt(k1 * k1 + k2 * k2))/2.0;
        d4X = p2.x - (int)(n/2.0);
        d4Y = p2.y + (int)(n * sqrt(3.0)/2.0);
        h = (sqrt(3.0)/2.0) * g;
        d2X = d4X + (int)((h * sqrt(3.0))/2.0);
        d2Y = d4Y + (int)(h/2.0);

        d3X = p2.x - (int)(g/2.0);
        d3Y = p2.y + (int)(g*sqrt(3.0)/2.0);
    } else {
        float k1 = (float)(p2.x - p1.x);
        float k2 = (float)(p2.y - p1.y);
        float g = (sqrt(k1 * k1 + k2 * k2))/3.0;

        d1X = p1.x + (int)(g/2.0);
        d1Y = p1.y + (int)(g * sqrt(3.0)/2.0);

        float n = (sqrt(k1 * k1 + k2 * k2))/2.0;
        d4X = p1.x + (int)(n/2.0);
        d4Y = p1.y + (int)(n * sqrt(3.0)/2.0);
        h = ((sqrt(3.0))/2.0) * g;
        d2X = d4X - (int)((h * sqrt(3.0))/2.0);
        d2Y = d4Y + (int)(h/2.0);

        d3X = p1.x + (int)(g);
        d3Y = p1.y + (int)(g * sqrt(3.0));
    }

    pp1.x = d1X;
    pp1.y = d1Y;
    pp1.index = 0;

    pp2.x = d2X;
    pp2.y = d2Y;
    pp2.index = 0;

    pp3.x = d3X;
    pp3.y = d3Y;
    pp3.index = 0;

    arrOfPoints[len - 3] = pp1;
    arrOfPoints[len - 2] = pp2;
    arrOfPoints[len - 1] = pp3;

    return arrOfPoints;
}


Point* findPointsDownKoha(Point p1, Point p2, Point* arrOfPoints, int len){
    Point pp1, pp2, pp3;
    int d1X, d1Y, d2X, d2Y, d3X, d3Y, d4X, d4Y;
    float h;

    if (p1.y == p2.y){
        int j = (int)(((float)(p2.x - p1.x))/3.0);

        d1X = p1.x + j;
        d1Y = p1.y;

        d2X = p1.x + (int)(((float)(p2.x - p1.x))/2.0);
        h = (sqrt(3.0)/2.0) * (((float)(p2.x - p1.x))/3.0);
        d2Y = p1.y - (int)(h);

        d3X = p1.x + 2 * j;
        d3Y = p1.y;
    } else if (p1.y > p2.y){
        float k1 = (float)(p2.x - p1.x);
        float k2 = (float)(p1.y - p2.y);
        float g = (sqrt(k1 * k1 + k2 * k2))/3.0;

        d1X = p1.x + (int)(g/2.0);
        d1Y = p1.y - (int)(g * sqrt(3.0)/2.0);

        float n = (sqrt(k1 * k1 + k2 * k2))/2.0;
        d4X = p1.x + (int)(n/2.0);
        d4Y = p1.y - (int)(n * sqrt(3.0)/2.0);
        h = ((sqrt(3.0))/2.0) * g;
        d2X = d4X - (int)((h * sqrt(3.0))/2.0);
        d2Y = d4Y - (int)(h/2.0);

        d3X = p1.x + (int)(g);
        d3Y = p1.y - (int)(g * sqrt(3.0));
    } else {
        float k1 = (float)(p2.x - p1.x);
        float k2 = (float)(p2.y - p1.y);
        float g = (sqrt(k1 * k1 + k2 * k2))/3.0;

        d1X = p2.x - (int)(g);
        d1Y = p2.y - (int)(g * sqrt(3.0));

        float n = (sqrt(k1 * k1 + k2 * k2))/2.0;
        d4X = p2.x - (int)(n/2.0);
        d4Y = p2.y - (int)(n * sqrt(3.0)/2.0);
        h = (sqrt(3.0)/2.0) * g;
        d2X = d4X + (int)((h * sqrt(3.0))/2.0);
        d2Y = d4Y - (int)(h/2.0);

        d3X = p2.x - (int)(g/2.0);
        d3Y = p2.y - (int)(g * sqrt(3.0)/2.0);
    }

    pp1.x = d1X;
    pp1.y = d1Y;
    pp1.index = 0;

    pp2.x = d2X;
    pp2.y = d2Y;
    pp2.index = 0;

    pp3.x = d3X;
    pp3.y = d3Y;
    pp3.index = 0;

    arrOfPoints[len - 3] = pp1;
    arrOfPoints[len - 2] = pp2;
    arrOfPoints[len - 1] = pp3;

    return arrOfPoints;
}

void drawLine(struct Png *image, png_byte r, png_byte g, png_byte b, png_byte a, int x1, int y1, int x2, int y2) {
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    int error = deltaX - deltaY;
    png_byte *row = image->row_pointers[y2];
    png_byte *ptr = &(row[x2 * 4]);
    ptr[0] = r;
    ptr[1] = g;
    ptr[2] = b;
    ptr[3] = a;
    while(x1 != x2 || y1 != y2)
    {
        png_byte *row = image->row_pointers[y1];
        png_byte *ptr = &(row[x1 * 4]);
        ptr[0] = r;
        ptr[1] = g;
        ptr[2] = b;
        ptr[3] = a;
        int error2 = error * 2;
        if(error2 > -deltaY)
        {
            error -= deltaY;
            x1 += signX;
        }
        if(error2 < deltaX)
        {
            error += deltaX;
            y1 += signY;
        }
    }

}

int compareIndex(const void* p1,const void* p2){
    Point pp1 = *((Point*)p1);
    Point pp2 = *((Point*)p2);
    if (pp1.index < pp2.index)
        return -1;
    if (pp1.index == pp2.index)
        return 0;
    return 1;
}

int compareX(const void* p1,const void* p2){
    Point pp1 = *((Point*)p1);
    Point pp2 = *((Point*)p2);
    if (pp1.x < pp2.x)
        return -1;
    if (pp1.x == pp2.x)
        return 0;
    return 1;
}

int compareY(const void* p1,const void* p2){
    Point pp1 = *((Point*)p1);
    Point pp2 = *((Point*)p2);
    if (pp1.y < pp2.y)
        return -1;
    if (pp1.y == pp2.y)
        return 0;
    return 1;
}



void drawUpKoha (struct Png *image, png_byte  r, png_byte  g, png_byte  b, png_byte a, Point aP, Point bP){
    int length = 5;
    Point* arrOfPointsUp = calloc(length, sizeof(Point));
    aP.index =  1;
    arrOfPointsUp[0] = aP;
    bP.index = 5;
    arrOfPointsUp[1] = bP;
    findPointsUpKoha(arrOfPointsUp[0], arrOfPointsUp[1], arrOfPointsUp, length);
    arrOfPointsUp[length - 3].index = 2;
    arrOfPointsUp[length - 2].index = 3;
    arrOfPointsUp[length - 1].index = 4;
    qsort(arrOfPointsUp, length, sizeof(Point), compareIndex);

    int l = length - 1;
    for(int i = 1; i < length; i++){
        arrOfPointsUp[i].index = arrOfPointsUp[i].index + i * 3;
    }

    int y = 2;
    for (int i = 0; i< l; i++){
        length = length + 3;
        arrOfPointsUp = realloc(arrOfPointsUp, length * sizeof(Point));
        findPointsUpKoha(arrOfPointsUp[i], arrOfPointsUp[i + 1], arrOfPointsUp, length);
        arrOfPointsUp[length - 3].index = y;
        y++;
        arrOfPointsUp[length - 2].index = y;
        y++;
        arrOfPointsUp[length - 1].index = y;
        y = y + 2;
    }

    qsort(arrOfPointsUp, length, sizeof(Point), compareIndex);

    l = length - 1;
    for (int ind = 0; ind < l; ind++){
        drawLine(image, r, g, b, a, arrOfPointsUp[ind].x, arrOfPointsUp[ind].y, arrOfPointsUp[ind + 1].x, arrOfPointsUp[ind + 1].y);
    }
}

void drawDownKoha (struct Png *image, png_byte  r, png_byte  g, png_byte  b, png_byte a, Point aP, Point bP){
    int length = 5;
    Point* arrOfPointsDown = calloc(length, sizeof(Point));
    aP.index =  1;
    arrOfPointsDown[0] = aP;
    bP.index = 5;
    arrOfPointsDown[1] = bP;
    findPointsDownKoha(arrOfPointsDown[0], arrOfPointsDown[1], arrOfPointsDown, length);
    arrOfPointsDown[length - 3].index = 2;
    arrOfPointsDown[length - 2].index = 3;
    arrOfPointsDown[length - 1].index = 4;
    qsort(arrOfPointsDown, length, sizeof(Point), compareIndex);

    int l = length - 1;
    for(int i = 1; i < length; i++){
        arrOfPointsDown[i].index = arrOfPointsDown[i].index + i * 3;
    }

    int y = 2;
    for (int i = 0; i < l; i++){
        length = length + 3;
        arrOfPointsDown = realloc(arrOfPointsDown, length * sizeof(Point));
        findPointsDownKoha(arrOfPointsDown[i], arrOfPointsDown[i + 1], arrOfPointsDown, length);
        arrOfPointsDown[length - 3].index = y;
        y++;
        arrOfPointsDown[length - 2].index = y;
        y++;
        arrOfPointsDown[length - 1].index = y;
        y = y + 2;
    }

    qsort(arrOfPointsDown, length, sizeof(Point), compareIndex);

    l = length - 1;
    for (int ind = 0; ind < l; ind++){
        drawLine(image, r, g, b, a, arrOfPointsDown[ind].x, arrOfPointsDown[ind].y, arrOfPointsDown[ind + 1].x, arrOfPointsDown[ind + 1].y);
    }
}

void drawUpAndDownSemicircle (struct Png *image, png_byte  r, png_byte  g, png_byte  b, png_byte a, Point aP, Point bP){
    int q, h, radius;
    radius = (int)(((float)(bP.x - aP.x))/2.0);
    h = radius + aP.x;
    Point cP;
    Point* arrOfPoints = calloc(2 * radius + 2, sizeof(Point));

    for (int j = 0; j <= radius; j++){
        q = (int)(sqrt(radius * radius - j * j)) + h;
        if (aP.y < image->height/2){
            cP.y =  j + aP.y;
        } else {
            cP.y = aP.y - j;
        }
        cP.x = q;
        arrOfPoints[j] = cP;
    }

    for (int j = 0; j <= radius; j++){
        q = h - (int)(sqrt(radius * radius - j * j));
        if (aP.y < image->height/2){
            cP.y =  j + aP.y;
        } else {
            cP.y = aP.y - j;
        }
        cP.x = q;
        arrOfPoints[radius + 1 + j] = cP;
    }

    qsort(arrOfPoints, 2 * radius + 2, sizeof(Point), compareX);

    for (int ind = 0; ind < 2 * radius + 1; ind++){
        drawLine(image, r, g, b, a, arrOfPoints[ind].x, arrOfPoints[ind].y, arrOfPoints[ind + 1].x, arrOfPoints[ind + 1].y);
    }

}

void drawLeftAndRightSemicircle (struct Png *image, png_byte  r, png_byte  g, png_byte  b, png_byte a, Point aP, Point bP){
    int q, k, radius;
    radius = (int)(((float)(bP.y - aP.y))/2.0);
    k = radius + aP.y;
    Point cP;
    Point* arrOfPoints = calloc(2 * radius + 2, sizeof(Point));

    for (int j = 0; j <= radius; j++){
        q = (int)(sqrt(radius * radius - j * j)) + k;
        if (aP.x < image->width/2){
            cP.x = j + aP.x;
        } else {
            cP.x = aP.x - j;
        }
        cP.y = q;
        arrOfPoints[j] = cP;
    }

    for (int j = 0; j <= radius; j++){
        q = k - (int)(sqrt(radius * radius - j * j));
        if (aP.x < image->width/2){
            cP.x = j + aP.x;
        } else {
            cP.x = aP.x - j;
        }

        cP.y = q;
        arrOfPoints[radius + 1 + j] = cP;
    }

    qsort(arrOfPoints, 2 * radius + 2, sizeof(Point), compareY);

    for (int ind = 0; ind < 2 * radius + 1; ind++){
        drawLine(image, r, g, b, a, arrOfPoints[ind].x, arrOfPoints[ind].y, arrOfPoints[ind + 1].x, arrOfPoints[ind + 1].y);
    }

}

void makeFrame(struct Png *image, png_byte  r, png_byte  g, png_byte  b, png_byte a, int thickness) {
    int frame, partUpAndDownSemicircle, partLeftAndRightSemicircle, partOfUpAndDownKoha, f;
    thickness = 0;
    printf("Введите номер рамки: 1, 2\n");
    scanf("%d", &frame);
    Point aP, bP;
    switch (frame){
        case 1:
            partOfUpAndDownKoha = (int)(((float)(image->width))/6.0);


            for(thickness = 0; thickness <= thickness; thickness++){
                f = 0;
                while (f < 6){
                    aP.x = f * partOfUpAndDownKoha;
                    aP.y = thickness;
                    bP.x = (f + 1) * partOfUpAndDownKoha;
                    bP.y = thickness;
                    drawUpKoha (image, r, g, b, a, aP, bP);
                    f++;
                }
            }

            for(thickness = 0; thickness <= thickness; thickness++){
                f = 0;
                while (f < 6){
                    aP.x = f * partOfUpAndDownKoha;
                    aP.y = image->height - 1 - thickness;
                    bP.x = (f + 1) * partOfUpAndDownKoha;
                    bP.y = image->height - 1 - thickness;
                    drawDownKoha (image, r, g, b, a, aP, bP);
                    f++;
                }
            }

            for(thickness = 0; thickness <= thickness; thickness++){
                drawLine(image, r, g, b, a, thickness, 0, thickness, image->height - 1);
            }

            for(thickness = 0; thickness <= thickness; thickness++){
                drawLine(image, r, g, b, a, image->width - 1 - thickness, 0, image->width - 1 - thickness, image->height - 1);
            }

            break;
        case 2:
            partUpAndDownSemicircle = (int)(((float)(image->width))/6.0);
            partLeftAndRightSemicircle = (int)(((float)(image->height))/6.0);
            thickness = 0;

            for (thickness = 0; thickness <= thickness; thickness++){
                f = 0;
                while (f < 6){
                    aP.x = f * partUpAndDownSemicircle;
                    aP.y = thickness;
                    bP.x = (f + 1) * partUpAndDownSemicircle;
                    bP.y = thickness;
                    drawUpAndDownSemicircle (image, r, g, b, a, aP, bP);
                    f++;
                }
            }

            for(thickness = 0; thickness <= thickness; thickness++){
                f = 0;
                while (f < 6){
                    aP.x = f * partUpAndDownSemicircle;
                    aP.y = image->height - 1 - thickness;
                    bP.x = (f + 1) * partUpAndDownSemicircle;
                    bP.y = image->height - 1 - thickness;
                    drawUpAndDownSemicircle (image, r, g, b, a, aP, bP);
                    f++;
                }
            }

            for(thickness = 0; thickness <= thickness; thickness++){
                f = 0;
                while (f < 6){
                    aP.x = thickness;
                    aP.y = f * partLeftAndRightSemicircle;
                    bP.x = thickness;
                    bP.y = (f + 1) * partLeftAndRightSemicircle;
                    drawLeftAndRightSemicircle (image, r, g, b, a, aP, bP);
                    f++;
                }
            }

            for(thickness = 0; thickness <= thickness; thickness++){
                f = 0;
                while (f < 6){
                    aP.x = image->width - 1 - thickness;
                    aP.y = f * partLeftAndRightSemicircle;
                    bP.x = image->width - 1 - thickness;
                    bP.y = (f + 1) * partLeftAndRightSemicircle;
                    drawLeftAndRightSemicircle (image, r, g, b, a, aP, bP);
                    f++;
                }
            }

            break;

        default:
            printf("Номера с такой рамкой нет\n");
    }
}

void createArrOfPositions(struct Png* image, png_byte find_r, png_byte find_g, png_byte find_b, png_byte find_a, int** position) {
    for (int y = 0; y < image->height; y++) {
        png_byte *row = image->row_pointers[y];
        for (int x = 0; x < image->width; x++) {
            png_byte *ptr = &(row[x * 4]);
            if (ptr[0] == find_r && ptr[1] == find_g && ptr[2] == find_b && ptr[3] == find_a) {
                position[y][x] = 1;
            } else if (ptr[0] != find_r || ptr[1] != find_g || ptr[2] != find_b || ptr[3] != find_a) {
                position[y][x] = 0;
            }
        }
    }
}

typedef struct Figure{
    Point top1; // верхняя левая вершина
    Point top2; // верхняя правая вершина
    Point top3; // нижняя правая вершина
    Point top4; // нижняя левая вершина
    Point maxTop; //координаты наибольшего x и y
    Point minTop; // координаты наименьшего x и y
} Figure;


void findVertices(int y, int x, Figure* vertices, int** position, int H, int W)
{
    Point el;
    el.x = x;
    el.y = y;

    Point* value = calloc(H*W*2, sizeof(Point));
    int count = 0;
    int top_index = -1;
    value[top_index+1] = el; top_index++; count++; //push
    position[el.y][el.x] = 2;
    while(count != 0)
    {
        el = value[top_index]; top_index--; count--; //pop
        position[el.y][el.x] = 2; //закрасить пиксель новым цветом

        if((el.y <= vertices->top1.y) && (el.x <= vertices->top1.x)) { //поиск ЛВ
            vertices->top1.x = el.x;
            vertices->top1.y = el.y;
        }

        if((el.y <= vertices->top2.y) && (el.x >= vertices->top2.x)) { //поиск ПВ
            vertices->top2.x = el.x;
            vertices->top2.y = el.y;
        }

        if((el.y >= vertices->top3.y) && (el.x >= vertices->top3.x)) { //поиск ПН
            vertices->top3.x = el.x;
            vertices->top3.y = el.y;
        }

        if((el.y >= vertices->top4.y) && (el.x <= vertices->top4.x)) { //поиск ЛН
            vertices->top4.x = el.x;
            vertices->top4.y = el.y;
        }

        if ((el.y <= vertices->minTop.y)){ //поиск самой низкой высоты
            vertices->minTop.y = el.y;
        }

        if ((el.x <= vertices->minTop.x)){ //поиск самой левой границы
            vertices->minTop.x = el.x;
        }

        if ((el.y >= vertices->maxTop.y)){ //поиск самой высокой высоты
            vertices->maxTop.y = el.y;
        }

        if ((el.x >= vertices->maxTop.x)){ //поиск самой правой границы
            vertices->maxTop.x = el.x;
        }

        if(el.x>0 && position[el.y][el.x-1]==1){
            el.x = el.x-1;
            el.y = el.y;
            value[top_index+1] = el; top_index++; count++; //push
        }
        if(el.x<(W-1) && position[el.y][el.x+1]==1){
            el.x = el.x+1;
            el.y = el.y;
            value[top_index+1] = el; top_index++; count++; //push
        }
        if(el.y>0 && position[el.y-1][el.x]==1){
            el.x = el.x;
            el.y = el.y-1;
            value[top_index+1] = el; top_index++; count++; //push
        }

        if(el.y<(H-1) && position[el.y+1][el.x]==1){
            el.x = el.x;
            el.y = el.y+1;
            value[top_index+1] = el; top_index++; count++; //push
        }
    }
    free(value);
}

long long int area(Figure vertices, int** position){
    long long int S = 0; //инициализация площади
    for (int i = vertices.minTop.y; i <  vertices.maxTop.y; i++){ //проходим по описанному прямоугольнику (вершины определены макс и мин границами фигуры)
        for (int j = vertices.minTop.x; j < vertices.maxTop.x; j++){
            if (position[i][j] == 2){ //если точка была пройдена в findVertices
                S++; // то учитываем ее как точку внутри фигуры
            }
        }
    }
    return S;
}

int isRectangle(Figure vertices, int S){
    if (vertices.top1.x != vertices.minTop.x || vertices.top1.y != vertices.minTop.y){ //вершина ЛВ не соответствует крайним границам фигуры
        return 0;
    }
    else if (vertices.top2.x != vertices.maxTop.x || vertices.top2.y != vertices.minTop.y){ // вершина ПВ не соответствует крайним границам фигуры
        return 0;
    }
    else if (vertices.top3.x != vertices.maxTop.x || vertices.top3.y != vertices.maxTop.y){ // вершина ПH не соответствует крайним границам фигуры
        return 0;
    }
    else if (vertices.top4.x != vertices.minTop.x || vertices.top4.y != vertices.maxTop.y){ // вершина ЛH не соответствует крайним границам фигуры
        return 0;
    }
    else if (S != ((vertices.maxTop.x - vertices.minTop.x) * (vertices.maxTop.y - vertices.minTop.y))){ //площадь, полученная исходя из вершин прямоугольника сверяется с площадью пройденной фигуры
        return 0;
    }
    else if (S == 0){
        return 0;
    }
    else
        return 1; //все Ок. Прямоугольник

}

void hightlightRectangle(struct Png* image, png_byte line_r, png_byte line_g, png_byte line_b, png_byte line_a, int thickness, Figure vertices){
    int xl = vertices.minTop.x - thickness; // левая граница описанного прямоугольника
    int xr = vertices.maxTop.x + thickness; // правая граница описанного прямоугольника
    int yu = vertices.maxTop.y + thickness; // верхняя граница описанного прямоугольника
    int yl = vertices.minTop.y - thickness; // нижняя граница описанного прямоугольника
    for (int i = yl; i<yu+1; i++) {
        png_byte *row = image->row_pointers[i];
        for (int j = xl; j<xr+1; j++) {
            png_byte *ptr = &(row[j * 4]);
            if (i < 0 || j < 0 || i >= image->height || j >= image->width) //на случай выхода за границу обводим ту часть, которая входит в рамки изображения
                continue;
            if (((i > vertices.maxTop.y) || (i < vertices.minTop.y)) || ((j < vertices.minTop.x) || (j > vertices.maxTop.x))){ //закрашивание только той области, которая лежит между описанным и исходным прямоугольником
                ptr[0] = line_r;
                ptr[1] = line_g;
                ptr[2] = line_b;
                ptr[3] = line_a;
            }
        }
    }
}


void findAllRectangle (struct Png* image, png_byte find_r, png_byte find_g, png_byte find_b, png_byte find_a, int thickness, png_byte line_r, png_byte line_g, png_byte line_b, png_byte line_a){
    int** position = (int**) calloc(image->height, sizeof(int*));
    for (int i = 0; i<image->height; i++){
        position[i] = (int*) calloc(image->width, sizeof(int));
    }

    createArrOfPositions(image, find_r, find_g, find_b, find_a, position);

    long long int S = 0; //площадь фигуры
    Figure vertices; //границы фигуры
    int flagRectangle = 0; //флажок для обозначения прямоугольника
    int counter = 0; //для учета случая, что ни один прямоугольник не найден
    printf("\n");
    printf("Координаты левого-верхнего и правого-нижнего угла найденных прямоугольников:\n");

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            if(position[i][j] == 1) {   //если нужная заливка
                vertices.top1.x = j; vertices.top1.y = i;
                vertices.top2.x = j; vertices.top2.y = i;
                vertices.top3.x = j; vertices.top3.y = i;
                vertices.top4.x = j; vertices.top4.y = i;
                vertices.minTop.x = j; vertices.minTop.y = i;
                vertices.maxTop.x = j; vertices.maxTop.y = i; //инициализация всех границ (для точки)
                findVertices(i, j, &vertices, position, image->height, image->width); //поиск всех границ и вершин
                S = area(vertices, position); //подсчет площади
                flagRectangle = isRectangle(vertices, S); //проверка фигуры на то, что он прямоугольник
                if (flagRectangle == 1){
                    counter++;
                    printf("Левый-верхний:%d %d, Правый-нижний:%d %d\n", vertices.top1.x, vertices.top1.y, vertices.top3.x, vertices.top3.y); //вывод левого верхнего, правого нижнего угла, площади прямоугольника
                    hightlightRectangle(image, line_r, line_g, line_b, line_a, thickness, vertices); //обводка прямоугольника
                }
                S = 0; //зануление площади для след фигуры
            }
        }
    }

    if (counter == 0){
        printf("Прямоугольников не найдено\n");
    }
    for (int i = 0; i<image->height; i++){
        free(position[i]);
    }
    free(position);
}

void choice(struct Configs *config, int opt) {
    int count;
    switch (opt) {
        case '1':
            count = sscanf(optarg, "%d,%d,%d,%d", &config->r, &config->g, &config->b,
                           &config->a); //для форматного ввода, считывание из строки
            if (count < 4) puts("-Some arguments were not read, so their default value did not change.");
            break;

        case '2':
            count = sscanf(optarg, "%d,%d,%d,%d", &config->line_r, &config->line_g, &config->line_b,
                           &config->line_a); //для форматного ввода, считывание из строки
            if (count < 4) puts("-Some arguments were not read, so their default value did not change.");
            break;

        case '3':
            count = sscanf(optarg, "%d,%d,%d,%d", &config->find_r, &config->find_g, &config->find_b,
                           &config->find_a); //для форматного ввода, считывание из строки
            if (count < 4) puts("-Some arguments were not read, so their default value did not change.");
            break;

        case '4':
            count = sscanf(optarg, "%d,%d,%d,%d", &config->r_cur, &config->g_cur, &config->b_cur,
                           &config->a_cur); //для форматного ввода, считывание из строки
            if (count < 4) puts("-Some arguments were not read, so their default value did not change.");
            break;

        case '5':
            count = sscanf(optarg, "%d,%d,%d,%d", &config->r_rep, &config->g_rep, &config->b_rep,
                           &config->a_rep); //для форматного ввода, считывание из строки
            if (count < 4) puts("-Some arguments were not read, so their default value did not change.");
            break;


        case 'r':
            config->replaceColor = 1;
            break;

        case 'm':
            config->makeFrame = 1;
            break;

        case 'f':
            config->findAllRectangle = 1;
            break;

        case 'h':
            printHelp();
            exit(0);

        default:
            puts("No such key.");
            break;
    }
}

int main(int argc, char **argv) {
    struct Png image;
    readPngFile(argv[1], &image);
    int opt;
    int longIndex;
    if (argc < 2) {
        puts("The program received too few arguments.");
        puts("Enter the key -h(--help) to see the instructions for the program.");
        return 0;
    }
    struct Configs config = {0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0,
                             0, 0};

    char *opts = "rmf1:2:3:4:5:h";
    struct option longOpts[] = {
            {"replaceColor", no_argument, NULL, 'r'},
            {"makeFrame", no_argument, NULL, 'm'},
            {"findAllRectangle", no_argument, NULL, 'f'},
            {"frameColors", required_argument, NULL, '1'},
            {"lineColors", required_argument, NULL, '2'},
            {"findColors", required_argument, NULL, '3'},
            {"curColors", required_argument, NULL, '4'},
            {"repColors", required_argument, NULL, '5'},
            {"help", no_argument, NULL, 'h'},
            {NULL, no_argument, NULL, 0},
    };
    opt = getopt_long(argc, argv, opts, longOpts, &longIndex);
    if (opt == -1) {
        puts("Invalid input format.");
        return 0;
    }
    while (opt != -1) {
        choice(&config, opt);
        opt = getopt_long(argc, argv, opts, longOpts, &longIndex);
    }
    if(config.replaceColor == 1){
        replaceColor(&image, config.r_cur, config.g_cur, config.b_cur, config.a_cur, config.r_rep, config.g_rep,
                     config.b_rep, config.a_rep);
    }
    if(config.makeFrame == 1){
        makeFrame(&image, config.r, config.g, config.b, config.a, config.thickness);
    }
    if(config.findAllRectangle == 1){
        findAllRectangle(&image, config.find_r, config.find_g, config.find_b, config.find_a,
                         config.thickness, config.line_r, config.line_g, config.line_b, config.line_a);
    }

    writePngFile(argv[2], &image);


    makeFrame(&image, 255, 0, 0, 255, 5);
    findAllRectangle (&image, 255, 0, 0, 255, 5, 0, 255, 0, 255);

    return 0;
}

