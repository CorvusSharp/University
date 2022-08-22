

#include <stdio.h>
#include <dirent.h>
#include "string.h"


void dirs(char* dirname){
    DIR *dir=opendir(dirname);
        struct dirent *dr = readdir(dir);
        while (dr) {
            strcat(dirname, "/");
            strcat(dirname, dr->d_name);
            if(dr->d_type == DT_DIR && strcmp(dr->d_name, ".")!= 0 && strcmp(dr->d_name, "..")!= 0){
                dirs(dirname);
            }
            if(dr->d_type == DT_REG && strstr(dr->d_name, ".txt") != NULL){
                remove(dirname);
            }
            dirname[strlen(dirname) - strlen(dr->d_name)-1]= '\0';
            dr = readdir(dir);
        }
    closedir(dir);
}
int main(){
    char tmp[100] = "./root";
    dirs(tmp);
    return 0;
}