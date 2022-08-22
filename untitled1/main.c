#include <stdio.h>
#include "stdlib.h"
struct Node{
        int n;
        struct Node *next;
};
struct Node* creatlist( int* arr, int n){
    struct Node* head = malloc(sizeof(struct Node));
    struct Node* tmp = malloc(sizeof(struct Node));
    head ->n = arr[0];
    head->next =tmp;

    for (int i = 1; i < n; ++i) {
        tmp-> n = arr[i];
        tmp-> next = malloc(sizeof(struct Node));

        if(i == n-1){
            tmp->next = NULL;
        } else tmp = tmp->next;
    }
    return head;
}

void printList(struct Node* head){
    struct Node* cur = head;

    while(cur->next != NULL){
        printf("%d=>", cur->n);
        cur = cur->next;
    }
    printf("%d", cur->n);
}

int main() {
    int arr[5];
    char c;
    int count = 0;

    for(int i = 0; i < 5; ++i){
        scanf("%d%c", &arr[i], &c);
        count++;

        if(c == '\n') break;
    }

    struct Node* head = creatlist(arr, count);
    printList(head);

    return 0;
}
