#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define N 6


struct Pairs{
    char word [100];
    char translate [100];
};

int main() {
    char c = '0';
    int memBlocks = 1;
    struct Pairs* dictionary = calloc(N*memBlocks, sizeof(struct Pairs));
    int k = 0;
    int i;
    int count = 0;
    while (5 == 5) {
        char text [100];
        c = '0';
        struct Pairs a;
        for (i = 0; c != ' ' && c != '\n'; i++) {
            scanf("%c", &c);
            text[i] = c;
        }
        text[i] = '\0';
        strcpy(a.word , text);
        if (strcmp(a.word, "ugabuda\0") == 0) {
            break;
        }
        char text2 [100];
        c = '0';
        for (i = 0; c != ' ' && c != '\n'; i++) {
            scanf("%c", &c);
            text2[i] = c;
        }
        text2[i] = '\0';
        strcpy(a.translate , text2);
        dictionary[k] = a;
        count++;
        if (k == N*memBlocks - 1) {
            memBlocks++;
            dictionary = realloc(dictionary, N*memBlocks* sizeof(struct Pairs));
        }
        k++;
    }
    c = '0';
    while (c != '\n') {
        c = '0';
        char text3 [100];
        for (i = 0; c != ' ' && c != '\n'; i++) {
            scanf("%c", &c);
            text3[i] = tolower(c);
        }
        text3[i] = '\0';
        int  m = 0;
        for (int f = 0; f< count; f++){
            if (strcmp(dictionary[f].word, text3) == 0) {
                printf("%s", dictionary[f].translate);
                break;
            } else {
                m++;
            }
        }
        if (m == count){
            printf("<unknown>");
        }
        if (c != '\n'){
            printf(" ");
        } else {
            break;
        }

    }
    free(dictionary);
    return 0;
}



