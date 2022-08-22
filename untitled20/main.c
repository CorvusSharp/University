#include <stdio.h>
#define razm 500
int stepen(const int* arr){
    int a = *(const int*)arr;
    return (a*a*a);
}

int multix2(const int* arr){
    return (*(const int*)arr*2);
}
int modul(const int* arr){
    int a = *(const int *)arr;
    if (a>=0) return a;
    return -a;
}

void forEachElement(int* arr, int n, int (*func)(const void*)){

for ( int i  = 0; i < n;++i){
    arr[i] = func(&arr[i]);
    }
}
int main() {
    int c = razm;
    int arr[razm];
    for(int i = 0; i< c; i++){
        scanf("%d", &arr[i]);
    }
    int opt;
    scanf("%d", &opt);
    switch (opt) {
        case 1:
            forEachElement(arr, c, (int (*)(const void *)) multix2);
            break;
        case 2:
            forEachElement(arr, c, (int (*)(const void *)) stepen);
            break;
        case 3:
            forEachElement(arr, c, (int (*)(const void *)) modul);
            break;
        default:
            printf("Error");
            return 0;
 }
    for (int j = 0; j < c-1; ++j) {
        printf("%d\n", arr[j]);
    }
    printf("%d\n", arr[c-1]);
    return 0;
}