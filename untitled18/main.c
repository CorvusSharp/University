#include "stdlib.h"

#include "stdio.h"

 int main(){
    int c = 0;
     for (int i = 0; i < 9400; ++i) {
         if(i%11 == 0 || i% 12 == 0 || i%9 == 0|| i%16 == 0){
    c++;
         }

     }
     printf("%d",c);
}