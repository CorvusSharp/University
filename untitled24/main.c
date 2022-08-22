#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <getopt.h>
#include <png.h>
void printHelp(){
    puts("\t\t\t\t\t█░▄▀ █░█ █▀▀▄ ▄▀▀ ▄▀▄ ░▒▄▀█░\n\t\t\t\t\t█▀▄░ █░█ █▐█▀ ░▀▄ █▀█ ▒█▄▄█▄\n\t\t\t\t\t▀░▀▀ ░▀░ ▀░▀▀ ▀▀░ ▀░▀ ░░░▒█░");

    puts("Это программа с CLI для редактирования png файлов");
    puts("Формат ввода: ./a.out [имя исходного файла] [функция] "
         "-[ключ1]/--[полный ключ1] [аргумент1] ...\n");
    puts("Функции/ключи:");
    puts("    -с/--color_change Функция, ищет самый встречаемый цвет. Соддержит ключ -1");
    puts("    -d/--draw_square Функция, Рисует квадрат. Квадрат можно залить, cоддержит ключи:\n\t-1\n\t-l\n\t-r\n\t-f");
    puts("    -S/--swap_sq Функция, меняет куски выбранной области местами.\n\t Возможен выбор опции:\n\t 0 - по кругу\n\t"
         " 1 - по диоганалям\n\t Содержит ключи:\n\t-l\n\t-r\n\t-o");
    puts("    -1/--color [<число>.<число>.<число>] - Цвет перекраски (RGB)");
    puts("    -h/--help - Вывод справки о работе программы.");
    puts("    -o/--option [Число] - Выбор опции функции");
    puts("    -i/--info  Выводит информацию о изображении");
    puts("    -r/--right_srcCoord [<x-координата>.<y-координата>]- Принимает координаты верхнего правого угла");
    puts("    -l/--left_srcCoord [<x-координата>.<y-координата>]- Принимает координаты верхнего левого угла");
    puts("    -t/--thickness [<число>] - Толщина линии");
    puts("    -f/--filling   [<число>] - Заливка квадрата. Принимает на вход:'1' или '0'");
    puts("    -a/--square_side [<число>] - Длина стороны квадрта");
    puts("    -k/--kursa4  - функция курсовой");

}
struct Configs {
    int a,l_w, c_r, c_g,c_b, filling,x1,x2,y1,y2,opt;
    int change_color, swap_sq,draw_square,help,info,kursa4;

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
int read_png_file(char *file_name, struct Png *image) {
    int x,y;
    char header[8];    // 8 is the maximum size that can be checked

    /* open file and test for it being a png */
    FILE *fp = fopen(file_name, "rb");
    if (!fp){
        puts("Не получилось открыть файл");
        return 1;
    }

    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8)){
        puts("Файл не является формата PNG");
        return 1;
    }

    image->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!image->png_ptr){
        puts("Не удалость считать структру файла");
        return 1;
    }

    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr){
        puts("Не удалось считать структуру информации файла");
        return 1;
    }

    png_init_io(image->png_ptr, fp);
    png_set_sig_bytes(image->png_ptr, 8);
    png_read_info(image->png_ptr, image->info_ptr);

    image->width = png_get_image_width(image->png_ptr, image->info_ptr);
    image->height = png_get_image_height(image->png_ptr, image->info_ptr);
    image->color_type = png_get_color_type(image->png_ptr, image->info_ptr);
    image->bit_depth = png_get_bit_depth(image->png_ptr, image->info_ptr);
    image->number_of_passes = png_set_interlace_handling(image->png_ptr);

    png_read_update_info(image->png_ptr, image->info_ptr);

    /* read file */
    if (setjmp(png_jmpbuf(image->png_ptr))){
        puts("Не удалось считать изображение");
        return 1;
    }

    image->row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * image->height);
    for (y = 0; y < image->height; y++)
        image->row_pointers[y] = (png_byte *) malloc(png_get_rowbytes(image->png_ptr, image->info_ptr));

    png_read_image(image->png_ptr, image->row_pointers);

    fclose(fp);
    return 0;
}
int write_png_file(char *file_name, struct Png *image) {
    int x,y;
    FILE *fp = fopen(file_name, "wb");
    if (!fp){
        puts ("Не удалось открыть файл");
        return 1;
    }

    image->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!image->png_ptr){
        puts("Не удалость считать структру файла");
        return 1;
    }

    image->info_ptr = png_create_info_struct(image->png_ptr);
    if (!image->info_ptr){
        puts("Не удалось считать структуру информации файла");
        return 1;
    }


    png_init_io(image->png_ptr, fp);


    png_set_IHDR(image->png_ptr, image->info_ptr, image->width, image->height,
                 image->bit_depth, image->color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(image->png_ptr, image->info_ptr);


    /* write bytes */
    if (setjmp(png_jmpbuf(image->png_ptr))){
        puts("Не удалось записать байты файла.");
    }

    png_write_image(image->png_ptr, image->row_pointers);


    /* end write */
    if (setjmp(png_jmpbuf(image->png_ptr))){
        puts("Не удалось записать файл.");
    }

    png_write_end(image->png_ptr, NULL);

    /* cleanup heap allocation */
    for (y = 0; y < image->height; y++)
        free(image->row_pointers[y]);
    free(image->row_pointers);

    fclose(fp);
    return 0;
}
void draw_square(struct Png *image, int x1, int y1, int a, int l_w, int c_r, int c_g, int c_b, int filling) {
    if (x1 < 0 || y1 < 0 || a < 0 || l_w < 0)
    {
        puts("Недопустимые параметры: \nКоординаты не могут быть отрицательными!\nТолщина линии не может быть отрицательной");
        return;
    }

    if (x1 > image->height || y1 > image->width)
    {
        puts("Недопустимые значения:\n Координаты квадрата не могут лежать за пределами нашего изображения");
        return;
    }
    if ((x1 + a) >= image->height || (y1 + a) >= image->width)
    {
        puts("Недопустимые параметры:\nКвадрат не может выходить за координаты изображения");
        return;
    }

    if (c_r > 255 || c_g > 255 || c_b > 255)
    {
        puts("\"Недопустимые параметры:\n недопустимое значения цвета!");
        return;
    }
    if (filling == 0) {
        for (int x = x1; x < x1 + a; x++) {
            png_bytep row = image->row_pointers[x];
            for (int y = y1; y < y1 + a; y++) {
                png_bytep ptr1 = &(row[y * 4]);
                if ((y - y1) < l_w || (x - x1) < l_w || ((y1 + a) - y) <= l_w || ((x1 + a) - x) <= l_w) {
                    ptr1[0] = c_r;
                    ptr1[1] = c_g;
                    ptr1[2] = c_b;
                    ptr1[3] = 255;
                }
            }
        }
    }
    if (filling == 1) {
        if (c_r > 255 || c_r < 0 || c_g > 255 || c_g < 0 || c_b > 255 || c_b < 0 ) {
            puts("Недопустимые параметры:\n недопустимое значения цвета!");
            return;
        }
        for (int x = x1+l_w; x < x1 +a -l_w; ++x) {
            png_bytep row = image->row_pointers[x];
            for (int y = y1 + l_w; y < y1 + a - l_w; ++y) {
                png_bytep ptr =  &(row[4 * y]);

                ptr[0] = c_r;
                ptr[1] = c_g;
                ptr[2] = c_b;
                ptr[3] = 255;
            }
        }
    } else if (filling != 0) {
        puts("Поменяйте значениение заливки на 0 или 1\n");
        return;
    }
}
void swap_sq(struct Png *image, struct Png *copy, int x1, int y1, int x2, int y2, int opt) {
    if (x1 > x2 || y1 > y2)
    {
        puts("Недопустимые параметры:\n Координаты левого верхнего угла не могут быть больше координат  верхнего правого угла.");
        return;
    }

    if (x1 > image->width || x2 > image->width || y1 > image->height || y2 > image->height)
    {
        puts("Недопустимые параметры:\nКоординаты  не могут лежать за пределами нашего изображения.");
        return;
    }

    if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0)
    {
        puts("Недопустимые параметры:\nКоординаты не могут быть отрицательными.");
        return;
    }

    int m_x = (x2 - x1) / 2;
    int m_y = (y2 - y1) / 2;
    if (opt == 0) {
        for (int x = x1; x < x1 + m_x; ++x) {
            png_bytep row = image->row_pointers[x];
            png_bytep row_ch = image->row_pointers[x + m_x];

            for (int y = y1; y < y1 + m_y; ++y) {
                png_bytep ptr = &(row[y * 4]);
                png_bytep ptr_ch = &(row_ch[y * 4]);

                ptr[0] = ptr_ch[0];
                ptr[1] = ptr_ch[1];
                ptr[2] = ptr_ch[2];
                ptr[3] = ptr_ch[3];
            }
        }
        for (int x = x1 + m_x; x < x2; ++x) {
            png_bytep row = image->row_pointers[x];
            png_bytep row_ch = image->row_pointers[x];

            for (int y = y1; y < y1 + m_y; ++y) {
                png_bytep ptr = &(row[y * 4]);
                png_bytep ptr_ch = &(row_ch[(y + m_y) * 4]);

                ptr[0] = ptr_ch[0];
                ptr[1] = ptr_ch[1];
                ptr[2] = ptr_ch[2];
                ptr[3] = ptr_ch[3];
            }
        }
        for (int x = x1; x < x1 + m_x; ++x) {
            png_bytep row = image->row_pointers[x + m_x];
            png_bytep row_ch = image->row_pointers[x];

            for (int y = y1 + m_y; y < y2; ++y) {
                png_bytep ptr = &(row[(y) * 4]);
                png_bytep ptr_ch = &(row_ch[(y) * 4]);

                ptr[0] = ptr_ch[0];
                ptr[1] = ptr_ch[1];
                ptr[2] = ptr_ch[2];
                ptr[3] = ptr_ch[3];
            }
        }
        for (int x = x1; x < x1 + m_x; ++x) {
            png_bytep row = image->row_pointers[x];
            png_bytep row_ch = copy->row_pointers[x];

            for (int y = y1; y < y1 + m_y; ++y) {
                png_bytep ptr = &(row[(y + m_y) * 4]);
                png_bytep ptr_ch = &(row_ch[(y) * 4]);

                ptr[0] = ptr_ch[0];
                ptr[1] = ptr_ch[1];
                ptr[2] = ptr_ch[2];
                ptr[3] = ptr_ch[3];
            }
        }
    }
    else if (opt == 1) {
        for (int x = x1; x < x1 + m_x; ++x) {
            png_bytep row = image->row_pointers[x];
            png_bytep row_ch = image->row_pointers[x + m_x];
            png_bytep row_copy = image->row_pointers[x + m_x];
            png_bytep row_ch_copy = copy->row_pointers[x];

            for (int y = y1; y < y1 + m_y; y++) {

                png_bytep ptr = &(row[(y) * 4]);
                png_bytep ptr_ch = &(row_ch[(y + m_y) * 4]);
                ptr[2] = ptr_ch[2];
                ptr[3] = ptr_ch[3];
                ptr[0] = ptr_ch[0];
                ptr[1] = ptr_ch[1];

                png_bytep ptr_copy = &(row_copy[(y + m_y) * 4]);
                png_bytep ptr_ch_copy = &(row_ch_copy[(y) * 4]);
                ptr_copy[0] = ptr_ch_copy[0];
                ptr_copy[1] = ptr_ch_copy[1];
                ptr_copy[2] = ptr_ch_copy[2];
                ptr_copy[3] = ptr_ch_copy[3];
            }
        }
        for (int x = x1 + m_x; x < x2; ++x) {
            png_bytep row = image->row_pointers[x];
            png_bytep row_ch = image->row_pointers[x - m_x];
            png_bytep row_copy = image->row_pointers[x - m_x];
            png_bytep row_ch_copy = copy->row_pointers[x];

            for (int y = y1; y < y1 + m_y; y++) {
                png_bytep ptr = &(row[(y) * 4]);
                png_bytep ptr_ch = &(row_ch[(y + m_y) * 4]);
                ptr[0] = ptr_ch[0];
                ptr[1] = ptr_ch[1];
                ptr[2] = ptr_ch[2];
                ptr[3] = ptr_ch[3];
                png_bytep ptr_copy = &(row_copy[(y + m_y) * 4]);
                png_bytep ptr_ch_copy = &(row_ch_copy[(y) * 4]);
                ptr_copy[0] = ptr_ch_copy[0];
                ptr_copy[1] = ptr_ch_copy[1];
                ptr_copy[2] = ptr_ch_copy[2];
                ptr_copy[3] = ptr_ch_copy[3];
            }
        }
    }
    else {
        puts("Опция может принимать только значение 0 или 1");
        return;
    }
}
int pix(struct Png* image, int r,int g,int b) {
    for (int y = 0; y < image->width;++y) {
        png_bytep row = image->row_pointers[y];
        for (int x = 0; y < image->height; x++) {
            png_bytep ptr = &(row[x * 4]);
            if (ptr[0] == 0 && ptr[1] == 11 && ptr[2] == 1)
                printf("X%d,y%d", x, y);
        }
    }
}
void kurs(struct Png* image) {
    int count_red = 0;
    int count_blue = 0;
    int count_green = 0;
    int count = 0;
    int c = 0;
    int ***cube_colors = calloc(256, sizeof(int **));

    for (int x = 0; x < 256; ++x) {
        cube_colors[x] = calloc(256, sizeof(int *));
        for (int y = 0; y < 256; ++y) {
            cube_colors[x][y] = calloc(256, sizeof(int));
        }
    }

    for (int x = 0; x < image->height; x++) {
        png_bytep row = image->row_pointers[x];
        for (int y = 0; y < image->width; y++) {
            png_bytep ptr = &(row[y * 4]);
            cube_colors[ptr[0]][ptr[1]][ptr[2]]++;
            c++;

        }

    }
    int colorRepeat = cube_colors[0][0][0];
    for (int x = 0; x < 256; ++x) {
        for (int y = 0; y < 256; ++y) {
            for (int z = 0; z < 256; ++z) {
                if (cube_colors[x][y][z]) {
                    count++;
                   printf("%d-%d-%d\n", x, y, z);
                }
            }
        }
    }
}
  //printf("%d\n", count);



void color_change(struct Png* image, int c_r, int c_g, int c_b) {
    int count_red = 0;
    int count_blue = 0;
    int count_green = 0;
    if (c_r < 0 || c_g < 0 || c_b < 0 || c_r > 255 || c_g > 255 || c_b > 255) {
        puts("Недопустимые параметры: \nВы ввели не коректное значение цвета\n");
        return;
    }
    int ***cube_colors = calloc(256, sizeof(int **));

    for (int x = 0; x < 256; ++x) {
        cube_colors[x] = calloc(256, sizeof(int *));
        for (int y = 0; y < 256; ++y) {
            cube_colors[x][y] = calloc(256, sizeof(int));
        }
    }

    for (int x = 0; x < image->height; x++) {
        png_bytep row = image->row_pointers[x];
        for (int y = 0; y < image->width; y++) {
            png_bytep ptr = &(row[y * 4]);
            cube_colors[ptr[0]][ptr[1]][ptr[2]]++;

        }
    }
    int colorRepeat =  cube_colors[0][0][0];
    for (int x = 0; x < 256; ++x) {
        for (int y = 0; y < 256; ++y) {
            for (int z = 0; z < 256; ++z) {
                if (colorRepeat < cube_colors[x][y][z]) {
                    count_red = x;
                    count_green = y;
                    count_blue = z;
                    colorRepeat = cube_colors[x][y][z];
                }
            }
        }
    }
    for (int x = 0; x < image->height; x++) {
        png_bytep row = image->row_pointers[x];
        for (int y = 0; y < image->width; y++) {
            png_bytep ptr = &(row[y * 4]);
            if(ptr[0] == count_red && ptr[1] == count_green && ptr[2] == count_blue){
                ptr[0] = c_r;
                ptr[1]= c_g;
                ptr[2] = c_b;
            }
        }
    }
    for (int y = 0; y < 256; y++) {
        for (int x = 0; x < 256; x++) {
            free(cube_colors[y][x]);
        }
    }
    for (int y = 0; y < 256; y++) {
        free(cube_colors[y]);
    }
    free(cube_colors);
}
void printtest(struct Png *image){
    puts("kek");
}
void choice(struct Configs* config, int opt){
    int count;
    switch (opt) {
        case '1':
            count = sscanf(optarg, "%d,%d,%d", &config->c_r, &config->c_g, &config->c_b); //для форматного ввода, считывание из строки
            if(count < 3) puts("-Some arguments were not read, so their default value did not change.");
            break;


        case 'c':
            config->change_color = 1;
            break;

        case 'd':
            config->draw_square = 1;
            break;

        case 'S':
            config->swap_sq = 1;
            break;

        case 'i':
            config->info = 1;
            break;
        case 'l':
            count = sscanf(optarg, "%d,%d", &config->x1, &config->y1);
            if(count < 2) puts("Some arguments were not read, so their default value did not change.");

            break;
        case 'o':
            count = sscanf(optarg, "%d",&config->opt);
            if(count < 1) puts("Some arguments were not read, so their default value did not change.");

            break;

        case 'r':
            count = sscanf(optarg, "%d,%d", &config->x2, &config->y2);
            if(count < 2) puts("Some arguments were not read, so their default value did not change.");
            break;

        case 't':
            count = sscanf(optarg, "%d", &config->l_w);
            if(count < 1) puts("Some arguments were not read, so their default value did not change.");
            break;

        case 'f':
            count = sscanf(optarg, "%d", &config->filling);
            if(count < 1) puts("Some arguments were not read, so their default value did not change.");
            break;

        case 'a':
            count = sscanf(optarg, "%d", &config->a);
            if(count < 1) puts("Some arguments were not read, so their default value did not change.");
            break;
        case 'k':
            config->kursa4 = 1;
            break;

        case 'h':
            printHelp();
            exit(0);
        default:
            puts("No such key.");
            break;
    }
}
void info(struct Png *image){
    printf("width:%d\nheight:%d\nbit depth:%d\ncolor type:%d\n",image->width,image->height, image->bit_depth, image->color_type);
}
int main(int argc, char **argv) {
   int r;
   int g;
   int b;
    char filename[50];
    char out_file[50];
    struct Png image;
    struct Png copy;
    int opt;
    int longIndex;
    if (argc < 2) {
        puts("The program received too few arguments.");
        puts("Enter the key -h(--help) to see the instructions for the program.");
        return 0;
    }
    strcpy(filename, argv[1]);
    strcpy(out_file, argv[argc - 1]);
    read_png_file(filename, &image);
    read_png_file(filename, &copy);
    kurs(&image);
    write_png_file(out_file, &image);
}

