#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#define PNG_DEBUG 3
#define N 2
#include <png.h>

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

void replaceColor(struct Png *image, png_byte *ptrCur, png_byte
*ptrReplace) {
    int x,y;
    if (png_get_color_type(image->png_ptr, image->info_ptr) ==
        PNG_COLOR_TYPE_RGB) {
        // Some error handling: input file is PNG_COLOR_TYPE_RGB but must
    }
    if (png_get_color_type(image->png_ptr, image->info_ptr) !=
        PNG_COLOR_TYPE_RGBA){
        // Some error handling: color_type of input file must be
        PNG_COLOR_TYPE_RGBA;
    }

    for (y = 0; y < image->height; y++) {
        png_byte *row = image->row_pointers[y];
        for (x = 0; x < image->width; x++) {
            png_byte *ptr = &(row[x * 4]);
            if (ptr[0] == ptrCur[0] && ptr[1] == ptrCur[1] && ptr[2] ==
                                                              ptrCur[2] && ptr[3] == ptrCur[3]){
                ptr[0] = ptrReplace[0];
                ptr[1] = ptrReplace[1];
                ptr[2] = ptrReplace[2];
                ptr[3] = ptrReplace[3];
            }
        }
    }
}

typedef struct Point {
    float x;
    float y;
} Point;

Point* func1(Point p1, Point p2, Point* arrOfPointsUp){
    Point pp1, pp2, pp3;
    float d1X, d1Y, d2X, d2Y, d3X, d3Y;
    if (p1.y == p2.y){
        d1X = (p2.x-p1.x)/3.0;
        d1Y = p1.y;
        d2X = d1X * 2.0;
        d2Y = p1.y;
        d3X = (p2.x-p1.x)/2.0;
        d3Y = p1.y + p1.x;
    }
    else if (p1.y > p2.y){
        float k1 = p2.x - p1.x;
        float k2 = p1.y - p2.y;
        float g = (sqrt((float)(k1*k1 + k2*k2)))/3.0;
        d1X = p2.x-g/2.0;
        d1Y = p2.x + g*sqrt(3.0)/2.0;
        d2X = p2.x-g;
        d2Y = p2.x + g*sqrt(3.0);
        float n = (sqrt((float)(k1*k1 + k2*k2)))/2.0;
        d3X = p2.x-n/2.0;
        d3Y = p2.y + n*sqrt(3.0)/2.0;
    }
    else {
        float k1 = p2.x - p1.x;
        float k2 = p2.y - p1.y;
        float g = (sqrt((float)(k1*k1 + k2*k2)))/3.0;
        d1X = p1.x+g/2.0;
        d1Y = p1.x + g*sqrt(3.0)/2.0;
        d2X = p1.x+g;
        d2Y = p1.x + g*sqrt(3.0);
        float n = (sqrt((float)(k1*k1 + k2*k2)))/2.0;
        d3X = p1.x+n/2.0;
        d3Y = p1.y + n*sqrt(3.0)/2.0;
    }
    pp1.x = d1X;
    pp1.y = d1Y;
    pp2.x = d2X;
    pp2.y = d2Y;
    pp3.x = d3X;
    pp3.y = d3Y;
    int count = sizeof(arrOfPointsUp)/sizeof(Point);
    arrOfPointsUp = realloc(arrOfPointsUp, (count+3)*sizeof(Point));
    arrOfPointsUp[count] = pp1;
    arrOfPointsUp[count+1] = pp2;
    arrOfPointsUp[count+2] = pp3;
    return arrOfPointsUp;
}

void drawLine(struct Png *image, png_byte *ptrReplace, float x1, float y1,
              float x2, float y2) {
    const float deltaX = abs(x2 - x1);
    const float deltaY = abs(y2 - y1);
    const float signX = x1 < x2 ? 1 : -1;
    const float signY = y1 < y2 ? 1 : -1;
    float error = deltaX - deltaY;
    png_byte *row = image->row_pointers[y2];
    png_byte *ptr = &(row[x2 * 4]);
    ptr[0] = ptrReplace[0];
    ptr[1] = ptrReplace[1];
    ptr[2] = ptrReplace[2];
    ptr[3] = ptrReplace[3];
    while(x1 != x2 || y1 != y2)
    {
        png_byte *row = image->row_pointers[y1];
        png_byte *ptr = &(row[x1 * 4]);
        ptr[0] = ptrReplace[0];
        ptr[1] = ptrReplace[1];
        ptr[2] = ptrReplace[2];
        ptr[3] = ptrReplace[3];
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

int compare(const void* p1,const void* p2){
    Point pp1 = *((Point*)p1);
    Point pp2 = *((Point*)p2);
    if (pp1.x < pp2.x)
        return -1;
    if (pp1.x == pp2.x)
        return 0;
    return 1;
}

void makeFrame(struct Png *image, png_byte *ptrReplace){
    int frame, length;
    Point* arrOfPointsUp;
    printf("Введите номер рамки: 1, 2\n");
    scanf("%d", &frame);
    switch (frame){
        case 1:
            arrOfPointsUp = calloc(N, sizeof(Point));
            Point aUp = {0.0, 0.0};
            arrOfPointsUp[0] = aUp;
            Point bUp = {image->width - 1.0, 0};
            arrOfPointsUp[1] = bUp;
            arrOfPointsUp = func1(arrOfPointsUp[0], arrOfPointsUp[1],
                                  arrOfPointsUp);
            length = sizeof(arrOfPointsUp)/sizeof(Point);
            qsort(arrOfPointsUp, length, sizeof(Point), compare);
            for (int i = 0; i<(length - 2); i++){
                arrOfPointsUp = func1(arrOfPointsUp[i],
                                      arrOfPointsUp[i+1], arrOfPointsUp);
            }
            length = sizeof(arrOfPointsUp)/sizeof(Point);
            qsort(arrOfPointsUp, length, sizeof(Point), compare);
            for (int i = 0; i<(length - 2); i++){
                arrOfPointsUp = func1(arrOfPointsUp[i],
                                      arrOfPointsUp[i+1], arrOfPointsUp);
            }
            length = sizeof(arrOfPointsUp)/sizeof(Point);
            qsort(arrOfPointsUp, length, sizeof(Point), compare);
            for (int i = 0; i<(length - 2); i++){
                arrOfPointsUp = func1(arrOfPointsUp[i],
                                      arrOfPointsUp[i+1], arrOfPointsUp);
            }
            length = sizeof(arrOfPointsUp)/sizeof(Point);
            qsort(arrOfPointsUp, length, sizeof(Point), compare);
            break;
        case 2:
            arrOfPointsUp = calloc(N, sizeof(Point));
            Point cUp = {0.0, 0.0};
            arrOfPointsUp[0] = cUp;
            Point dUp = {image->width - 1.0, 0};
            arrOfPointsUp[1] = dUp;
            arrOfPointsUp = func1(arrOfPointsUp[0], arrOfPointsUp[1],
                                  arrOfPointsUp);
            length = sizeof(arrOfPointsUp)/sizeof(Point);
            qsort(arrOfPointsUp, length, sizeof(Point), compare);
            for (int i = 0; i<(length - 2); i++){
                arrOfPointsUp = func1(arrOfPointsUp[i],
                                      arrOfPointsUp[i+1], arrOfPointsUp);
            }
            length = sizeof(arrOfPointsUp)/sizeof(Point);
            qsort(arrOfPointsUp, length, sizeof(Point), compare);
            for (int i = 0; i<(length - 2); i++){
                arrOfPointsUp = func1(arrOfPointsUp[i],
                                      arrOfPointsUp[i+1], arrOfPointsUp);
            }
            length = sizeof(arrOfPointsUp)/sizeof(Point);
            qsort(arrOfPointsUp, length, sizeof(Point), compare);
            break;
        default:
            printf("Номера с такой рамкой нет\n");
    }
    int len = sizeof(arrOfPointsUp)/sizeof(Point);
    for (int m = 0; m < (len - 1); m++){
        drawLine(image, ptrReplace, arrOfPointsUp[m].x,
                 arrOfPointsUp[m].y, arrOfPointsUp[m+1].x, arrOfPointsUp[m+1].y);
    }
}

/*void func3(struct Png *image, png_byte *ptrRectangle, int w, png_byte
*ptrLine){
    int array[2];
    for (y = 0; y < image->height; y++) {
        int count = 0;
        int firstX;
        png_byte *row = image->row_pointers[y];
        for (x = 0; x < image->width; x++) {
            png_byte *ptr = &(row[x * 4]);
            if (ptr[0] == ptrRectangle[0] && ptr[1] == ptrRectangle[1] &&
ptr[2] == Rectangle[2] && ptr[3] == ptrRectangle[3]){
                if (count == 0){
                    firstX = x;
                }
                count++;
                if (x != (firstX + count -1)){

                }
                ptr[0] = ptrReplace[0];
                ptr[1] = ptrReplace[1];
                ptr[2] = ptrReplace[2];
                ptr[3] = ptrReplace[3];
            }
        }
    }
}
*/

int main(int argc, char **argv) {

    if (argc != 3){
        fprintf(stderr,"Usage: program_name <file_in> <file_out>\n");
        return 0;
    }

    struct Png image;
    read_png_file(argv[1], &image);
    replaceColor(&image);
    write_png_file(argv[2], &image);

    return 0;
}