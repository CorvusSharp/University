#include <stdio.h>
#include <dirent.h>
#include "string.h"
#include <stdlib.h>

void dirTraveler(const char *startDir, char* str, FILE* file){
    char path[1000];
    strcpy(path, startDir);
    DIR *dir=opendir(path);
    if(dir) {
        struct dirent *dr = readdir(dir);
        while (dr) {          // если это удалось
            if (dr->d_name[0] != '.') {
                if (path[strlen(path) - 1] != '/') {
                    path[strlen(path) + 1] = '\0';
                    path[strlen(path)] = '/';
                }
                char dirPath_add[300];
                strcpy(dirPath_add, path);
                strcat(dirPath_add, dr->d_name);
                if (str[0] == dr->d_name[0] && dr->d_name[1] == '.') {
                    fprintf(file, "%s%s\n", path, dr->d_name);
                } else {
                    dirTraveler(dirPath_add, str, file);
                }
            }
            dr = readdir(dir);
        }
    }
    closedir(dir);
}





int main(){
    char tmp[100] = "./tmp";
    FILE *file;
    file = fopen("./result.txt","w");
    char* str = (char*)malloc(30 * sizeof(char));
    fgets(str, 30, stdin);
    int len = (int)strlen(str);
    for(int i = 0;i<len;i++){
        dirTraveler(tmp, str + i, file);
    }
    fclose(file);
    free(str);
    return 0;
}