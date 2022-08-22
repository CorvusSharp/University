#include <stdio.h>
#include <string.h>


int count(char* str){
    char* pc;
    char string[201];
    char* arr[100];
    int k = 0;
    int max = 0;
    int m = 0;
    int n, p;
    strcpy(string, str);
    pc = strtok (str, " ,.-\n");
    while (pc != NULL)
    {
        arr[m] = pc;
        ++m;
        pc = strtok (NULL, " ,.-\n");
    }
    for(n = 0; n < m; ++n){
        k = 0;
        for(p = 0; p < m; ++p){
            if(!strcmp(arr[n], arr[p])) {
                ++k;
            }
        }
        if(k > max) {
            max = k;
        }
    }
    return max;
}
int main() {
    char str[201];
    fgets(str, 201, stdin);
    int res = count(str);
    printf("%d", res);
    assertIsNotZero(res);
    return 0;
}