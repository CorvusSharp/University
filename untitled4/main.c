#include <stdio.h>
#include "stdlib.h"

struct Node{
    int n;
    struct Node *next;
};
struct Node* clist(int *arr, int n){
    struct Node* head  = malloc(sizeof(struct Node));
    struct Node* tmp = malloc(sizeof(struct Node));

    head -> next = tmp;

    for (int i= 0; i<n; i++){
        tmp->n = arr[i];
        tmp->next = malloc(sizeof(struct Node));
        if (i == n-1){
            tmp->next = NULL;
        } else tmp = tmp->next;
    }
    return head;
}
int Max(struct Node* head) {
    struct Node* cur = head;
    int max = -2147483648;
    while(cur->next != NULL){
        if(max< cur->n) max = cur-> n;
        cur = cur->next;
    }
    if(cur->n > max) max = cur->n;
    return max;
}

int main() {
    int arr[10];
    char p;
    int count = 0;
    for (int i = 0; i < 10; ++i) {
        scanf("%d%c",&arr[i],&p);
        count++;
        if(p == '\n')  break;

    }
    struct Node* head = clist(arr,count);
    int max = Max(head);
    printf("%d", max);
    return 0;
}
