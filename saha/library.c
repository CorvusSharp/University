#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct MusicalComposition {
    char* name;
    char* author;
    int year;
    struct MusicalComposition* prev;
    struct MusicalComposition* next;
} MusicalComposition;


// Создание структуры MusicalComposition
MusicalComposition* createMusicalComposition(char* name, char* author,int year){
    MusicalComposition* cur = (MusicalComposition*)malloc(sizeof(MusicalComposition));
    cur->name = name;
    cur->author = author;
    cur->year = year;
    cur->prev = NULL;
    cur->next = NULL;
    return cur;
};


// Функции для работы со списком MusicalComposition

MusicalComposition* createMusicalCompositionList(char** array_names, char** array_authors, int* array_years, int n) {
    MusicalComposition *prev;
    MusicalComposition *head;
    MusicalComposition *song;
    for (int i = 0; i < n; i++) {
        song = createMusicalComposition(array_names[i], array_authors[i], array_years[i]);
        song->name = array_names[i];
        song->author = array_authors[i];
        song->year = array_years[i];
        if ((i != 0) && (i != n - 1)) {
            song->prev = prev;
            prev->next = song;
            prev = song;
        } else {
            if (i == 0) {
                head = song;
                prev = song;
            } else {
                song->prev = prev;
                prev->next = song;
            }
        }
    }
    return head;
};


/*MusicalComposition* createMusicalCompositionList(char** array_names, char** array_authors, int* array_years, int n){
    int  i = 0;
    MusicalComposition** k = (MusicalComposition**)calloc(n, sizeof(MusicalComposition*));
    MusicalComposition* head;

    while (i<n){
        MusicalComposition* music = createMusicalComposition(array_names[i], array_authors[i], array_years[i]);
        k[i] = music;
        if (i == 0){
            head = music;
        }
        if (i != 0){
            k[i-1]->next = k[i];
            music->prev = k[i-1];
        }
        i++;
    }
    return head;
};*/


void push(MusicalComposition* head, MusicalComposition* element){
    MusicalComposition * now = head;
    while(now->next != NULL){
        now = now->next;
    }
    now->next = element;
};

void removeEl(MusicalComposition* head, char* name_for_remove){
    while(head->name != name_for_remove){
        head = head->next;
    }
    if (head->prev != NULL){
        head->prev->next = head->next;
    }
    if (head->next != NULL){
        head->next->prev = head->prev;
    }
    free(head);
};


int count(MusicalComposition* head){
    int cnt = 0;
    while(head){
        cnt++;
        head = head->next;
    }
    return cnt;
};


void print_names(MusicalComposition* head){
    while(head){
        printf("%s\n", head->name);
        head = head->next;
    }
};



int main(){
    int length;
    scanf("%d\n", &length);

    char** names = (char**)malloc(sizeof(char*)*length);
    char** authors = (char**)malloc(sizeof(char*)*length);
    int* years = (int*)malloc(sizeof(int)*length);

    for (int i=0;i<length;i++)
    {
        char name[80];
        char author[80];

        fgets(name, 80, stdin);
        fgets(author, 80, stdin);
        fscanf(stdin, "%d\n", &years[i]);

        (*strstr(name,"\n"))=0;
        (*strstr(author,"\n"))=0;

        names[i] = (char*)malloc(sizeof(char) * (strlen(name)+1));
        authors[i] = (char*)malloc(sizeof(char) * (strlen(author)+1));

        strcpy(names[i], name);
        strcpy(authors[i], author);

    }
    MusicalComposition* head = createMusicalCompositionList(names, authors, years, length);
    char name_for_push[80];
    char author_for_push[80];
    int year_for_push;

    char name_for_remove[80];

    fgets(name_for_push, 80, stdin);
    fgets(author_for_push, 80, stdin);
    fscanf(stdin, "%d\n", &year_for_push);
    (*strstr(name_for_push,"\n"))=0;
    (*strstr(author_for_push,"\n"))=0;

    MusicalComposition* element_for_push = createMusicalComposition(name_for_push, author_for_push, year_for_push);

    fgets(name_for_remove, 80, stdin);
    (*strstr(name_for_remove,"\n"))=0;

    printf("%s %s %d\n", head->name, head->author, head->year);
    int k = count(head);

    printf("%d\n", k);
    push(head, element_for_push);

    k = count(head);
    printf("%d\n", k);

    removeEl(head, name_for_remove);
    print_names(head);

    k = count(head);
    printf("%d\n", k);

    for (int i=0;i<length;i++){
        free(names[i]);
        free(authors[i]);
    }
    free(names);
    free(authors);
    free(years);

    return 0;

}
