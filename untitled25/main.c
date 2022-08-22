#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#define PNG_DEBUG 3
#include "png.h"

struct Png{
    int width, height;
    png_byte color_type;
    png_byte bit_depth;

    png_structp png_ptr;
    png_infop info_ptr;
    int number_of_passes;
    png_bytep *row_pointers;
};

void read_png_file(char *file_name, struct Png *image) {
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

void write_png_file(char *file_name, struct Png *image) {
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

void makeFrame(struct Png *image, png_byte  r, png_byte  g, png_byte  b, png_byte a, int d){
    int frame, partUpAndDownSemicircle, partLeftAndRightSemicircle, partOfUpAndDownKoha, f;
    int thickness = 0;
    printf("Введите номер рамки: 1, 2\n");
    scanf("%d", &frame);
    Point aP, bP;
    switch (frame){
        case 1:
            partOfUpAndDownKoha = (int)(((float)(image->width))/6.0);

            for(thickness = 0; thickness <= d; thickness++){
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

            for(thickness = 0; thickness <= d; thickness++){
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

            for(thickness = 0; thickness <= d; thickness++){
                drawLine(image, r, g, b, a, thickness, 0, thickness, image->height - 1);
            }

            for(thickness = 0; thickness <= d; thickness++){
                drawLine(image, r, g, b, a, image->width - 1 - thickness, 0, image->width - 1 - thickness, image->height - 1);
            }

            break;
        case 2:
            partUpAndDownSemicircle = (int)(((float)(image->width))/6.0);
            partLeftAndRightSemicircle = (int)(((float)(image->height))/6.0);
            thickness = 0;

            for (thickness = 0; thickness <= d; thickness++){
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

            for(thickness = 0; thickness <= d; thickness++){
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

            for(thickness = 0; thickness <= d; thickness++){
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

            for(thickness = 0; thickness <= d; thickness++){
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

void createArrPosition(struct Png* image, png_byte find_r, png_byte find_g, png_byte find_b, png_byte find_a, int** position) {
    for (int y = 0; y < image->height; y++) {
        png_byte *row = image->row_pointers[y];
        for (int x = 0; x < image->width; x++) {
            png_byte *ptr = &(row[x * 4]);
            if (ptr[0] == find_r && ptr[1] == find_g && ptr[2] == find_b && ptr[3] == find_a) {
                position[x][y] = 1;
            } else {
                position[x][y] = 0;
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


void fill(int y, int x, Figure* border, int** position, int H, int W)
{
    Point el;
    el.x = x;
    el.y = y;

    Point* value = malloc(sizeof(Point)*H*W*2);
    int count = 0;
    int top_index = -1;
    value[top_index+1] = el; top_index++; count++; //push
    position[el.y][el.x] = 2;
    while(count != 0)
    {
        el = value[top_index]; top_index--; count--; //pop
        position[el.y][el.x] = 2; //закрасить пиксель новым цветом

        if((el.y <= border->top1.y) && (el.x <= border->top1.x)) { //поиск ЛВ
            border->top1.x = el.x;
            border->top1.y = el.y;
        }

        if((el.y <= border->top2.y) && (el.x >= border->top2.x)) { //поиск ПВ
            border->top2.x = el.x;
            border->top2.y = el.y;
        }

        if((el.y >= border->top3.y) && (el.x >= border->top3.x)) { //поиск ПН
            border->top3.x = el.x;
            border->top3.y = el.y;
        }

        if((el.y >= border->top4.y) && (el.x <= border->top4.x)) { //поиск ЛН
            border->top4.x = el.x;
            border->top4.y = el.y;
        }

        if ((el.y <= border->minTop.y)){ //поиск самой низкой высоты
            border->minTop.y = el.y;
        }

        if ((el.x <= border->minTop.x)){ //поиск самой левой границы
            border->minTop.x = el.x;
        }

        if ((el.y >= border->maxTop.y)){ //поиск самой высокой высоты
            border->maxTop.y = el.y;
        }

        if ((el.x >= border->maxTop.x)){ //поиск самой правой границы
            border->maxTop.x = el.x;
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

void search_tops(int y, int x, Figure* border, int** position, int H, int W){
    position[y][x] = 2; //пройденную точку 1 помечаем 2
    if((y <= border->top1.y) && (x <= border->top1.x)) { //поиск ЛВ
        border->top1.x = x;
        border->top1.y = y;
    }
    if((y <= border->top2.y) && (x >= border->top2.x)) { //поиск ПВ
        border->top2.x = x;
        border->top2.y = y;
    }
    if((y >= border->top3.y) && (x >= border->top3.x)) { //поиск ПН
        border->top3.x = x;
        border->top3.y = y;
    }
    if((y >= border->top4.y) && (x <= border->top4.x)) { //поиск ЛН
        border->top4.x = x;
        border->top4.y = y;
    }

    if ((y <= border->minTop.y)){ //поиск самой низкой высоты
        border->minTop.y = y;
    }
    if ((x <= border->minTop.x)){ //поиск самой левой границы
        border->minTop.x = x;
    }
    if ((y >= border->maxTop.y)){ //поиск самой высокой высоты
        border->maxTop.y = y;
    }
    if ((x >= border->maxTop.x)){ //поиск самой правой границы
        border->maxTop.x = x;
    }

    //рекурсивно проходимся по соседним точкам, проверяя, что они входят в границы изображения
    if((x < (W-1)) && (position[y][x+1] == 1)){
        search_tops(y,x+1,border,position, H, W);
    }
    if((y < (H-1)) && (position[y+1][x] == 1)){
        search_tops(y+1,x,border,position, H, W);
    }
    if((x > 0) && (position[y][x-1] == 1)){
        search_tops(y,x-1,border,position, H, W);
    }
    if((y > 0) && (position[y-1][x] == 1)){
        search_tops(y-1,x,border,position, H, W);
    }

}


long long int count_area(Figure border, int** position){
    long long int S = 0; //инициализация площади
    for (int i = border.minTop.y; i <  border.maxTop.y; i++){ //проходим по описанному прямоугольнику (вершины определены макс и мин границами фигуры)
        for (int j = border.minTop.x; j < border.maxTop.x; j++){
            if (position[i][j] == 2){ //если точка была пройдена в search_tops
                S++; // то учитываем ее как точку внутри фигуры
            }
        }
    }
    return S;
}

int check_rectangle(Figure border, int S){
    if (border.top1.x != border.minTop.x || border.top1.y != border.minTop.y){ //вершина ЛВ не соответствует крайним границам фигуры
        return 0;
    }
    else if (border.top2.x != border.maxTop.x || border.top2.y != border.minTop.y){ // -//-
        return 0;
    }
    else if (border.top3.x != border.maxTop.x || border.top3.y != border.maxTop.y){ // -//-
        return 0;
    }
    else if (border.top4.x != border.minTop.x || border.top4.y != border.maxTop.y){ // -//-
        return 0;
    }
    else if (S != ((border.maxTop.x - border.minTop.x) * (border.maxTop.y - border.minTop.y))){ //площадь, полученная исходя из вершин прямоугольника сверяется с площадью пройденной фигуры
        return 0;
    }
    else if (S == 0){
        return 0;
    }
    else
        return 1; //все Ок. Прямоугольник

}

void hightlightRectangle(struct Png* image, png_byte line_r, png_byte line_g, png_byte line_b, png_byte line_a, int thickness, Figure border){
    int xl = border.minTop.x - thickness; // левая граница описанного прямоугольника
    int xr = border.maxTop.x + thickness; // правая -//-
    int yu = border.maxTop.y + thickness; //верхняя граница описанного прямоугольника
    int yl = border.minTop.y - thickness; // нижняя граница описанного прямоугольника
    for (int i = yl; i<yu+1; i++) {
        png_byte *row = image->row_pointers[i];
        for (int j = xl; j<xr+1; j++) {
            png_byte *ptr = &(row[j * 4]);
            if (i < 0 || j < 0 || i >= image->height || j >= image->width) //на случай выхода за границу обводим ту часть, которая входит в рамки изображения
                continue;
            if (((i > border.maxTop.y) || (i < border.minTop.y)) || ((j < border.minTop.x) || (j > border.maxTop.x))){ //закрашивание только той области, которая лежит между описанным и исходным прямоугольником
                ptr[0] == line_r;
                ptr[1] == line_g;
                ptr[2] == line_b;
                ptr[3] == line_a;
            }
        }
    }
}


void funca (struct Png* image, png_byte find_r, png_byte find_g, png_byte find_b, png_byte find_a, int thickness, png_byte line_r, png_byte line_g, png_byte line_b, png_byte line_a){
    int** position = (int**) calloc(image->height, sizeof(int*));
    for (int i = 0; i<image->height; i++){
        position[i] = (int*) calloc(image->width, sizeof(int));
    }
    createArrPosition(image, find_r, find_g, find_b, find_a, position);
    long long int S = 0; //площадь фигуры
    Figure border; //границы фигуры
    int flag_rectangle = 0; //флажок для обозначения прямоугольника
    int counter = 0; //для учета случая, что ни один прямоугольник не найден
    printf("\n");
    printf("Координаты левого-верхнего и правого-нижнего угла найденных прямоугольников:\n");

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            if(position[i][j] == 1) {   //если нужная заливка
                border.top1.x = j; border.top1.y = i;
                border.top2.x = j; border.top2.y = i;
                border.top3.x = j; border.top3.y = i;
                border.top4.x = j; border.top4.y = i;
                border.minTop.x = j; border.minTop.y = i;
                border.maxTop.x = j; border.maxTop.y = i; //инициализация всех границ (для точки)
                fill(i, j, &border, position, image->height, image->width); //поиск всех границ и вершин
                S = count_area(border, position); //подсчет площади

                flag_rectangle = check_rectangle(border, S); //проверка фигуры на то, что он прямоугольник
                if (flag_rectangle == 1){
                    counter++;
                    printf("Левый-верхний:%d %d, Правый-нижний:%d %d\n", border.top1.x, border.top1.y, border.top3.x, border.top3.y); //вывод левого верхнего, правого нижнего угла, площади прямоугольника
                    hightlightRectangle(image, line_r, line_g, line_b, line_a, thickness, border); //обводка прямоугольника
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


int main(int argc, char **argv) {

    if (argc != 3){
        fprintf(stderr,"Usage: program_name <file_in> <file_out>\n");
        return 0;
    }

    struct Png image;
    read_png_file(argv[1], &image);
//    makeFrame(&image, 255, 0, 0, 255, 5);
    funca (&image, 255, 0, 0, 255, 5, 0, 255, 0, 255);
    write_png_file(argv[2], &image);
    return 0;
}
