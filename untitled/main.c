#include <stdio.h>
#include <math.h>
#include "dirent.h"
void readFile(char *filename){
    FILE *f = fopen(filename, "r");
    char s[100];
    while(fgets(s, 100, f)){
        puts(s);
    }

    fclose(f);
}
void dirtrave(char *dirname){}
void fileCopy(char* filename, char* outfilename){
    FILE *f = fopen(filename, "r");
    FILE *f_out = fopen(outfilename, "w");
    char s[100];
    while(fgets(s, 100, f)){
        fprintf(f_out, ":%s", s);
    }

    fclose(f);
    fclose(f_out);
}

int main(){
    char* filename = "test.txt";
    char* outfilename = "out.txt";

    fileCopy(filename, outfilename);
    readFile(outfilename);

    DIR *dir = opendir(".");

    struct dirent *cur;
    while(cur = readdir(dir)){
        if(cur->d_type == DT_DIR){
            printf("DIR:\t");
        }
        printf("%s\n",cur->d_name);
    }
}