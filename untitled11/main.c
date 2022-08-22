#include <stdio.h>
#include "stdlib.h"
#include "math.h"
int count_if(void* base, size_t num, size_t size, int (*pred)(const void*)) {
    int counter = 0;
    void *cur = base;
    for (int i = 0; i < num; ++i) {
        if(pred(cur) == 1){
            counter++;
        }
        cur = base+i*size;
    }
    return counter;
}
int pred_int(const void* a){
    int cur = *(int*)a;
    if(abs(cur%2) == 0){
        return 0;
    }
    return 1;
}int pred_doub(const void* a){
    double cur = *(double*) a;
    if ((abs((int)(cur)%2)==0) && ((fabs(cur) - abs((int)cur)) < 0.000001)) return 0;
    return 1;
}
int main(){
    int n = 5;
    int size = 0;
    scanf("%d", &size);

    if(size == 4){
        int input_array[n];
        for(int i=0;i<n;i++)
            scanf("%d", &input_array[i]);
        int p = count_if(input_array, n, sizeof(int), pred_int);
        printf("%d", p);
    }

    if(size == 8){
        double input_array[n];
        for(int i=0;i<n;i++)
            scanf("%lf", &input_array[i]);
        int p = count_if(input_array, n, sizeof(double), pred_doub);
        printf("%d", p);
    }

    return 0;
}
