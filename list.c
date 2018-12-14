#include <stdio.h>
#include <stdlib.h>

typedef struct _ListNode{
    void* value;
    struct _ListNode *next, *previous;
} ListNode;

typedef struct{
    ListNode *front, *back;
    unsigned int size;
} List;

void listInitialize(List *li){
    li->front = li->back = (void*)(li->size = 0);
}

void* listGetBack(List *li){
    if(li->size==0)
        return 0;
    return li->back->value;
}

void* listGetFront(List *li){
    if(li->size==0)
        return 0;
    return li->front->value;
}

void listSetBack(List *li, void* value){
    if(li->size==0)
        return;
    li->back->value = value;
}

void listSetFront(List *li, void* value){
    if(li->size==0)
        return;
    li->front->value = value;
}

void listPushBack(List *li, void* value){
    ListNode *node = (ListNode*)malloc(sizeof(ListNode));
    node->value = value;
    node->previous = node->next = 0;

    if(li->size==0){
        li->front = li->back = node;
    }else{
        li->back->next = node;
        node->previous = li->back;
        li->back = node;
    }
    li->size += 1;
}

void listPushFront(List *li, void* value){
    ListNode *node = (ListNode*)malloc(sizeof(ListNode));
    node->value = value;
    node->previous = node->next = 0;

    if(li->size==0){
        li->front = li->back = node;
    }else{
        li->front->previous = node;
        node->next = li->front;
        li->front = node;
    }
    li->size += 1;
}

void* listPopBack(List *li){
    if(li->size==0)
        return;
    void* value = li->back->value;
    ListNode *newBack = li->back->previous;
    free(li->back);
    newBack->next = 0;
    li->back = newBack;
    return value;
}

void* listPopFront(List *li){
    if(li->size==0)
        return;
    void* value = li->front->value;
    ListNode *newFront = li->front->next;
    free(li->front);
    newFront->previous = 0;
    li->front = newFront;
    return value;
}

void listClear(List *li){
    if(li->size>0){
        ListNode *temp = li->front->next;
        while(temp!=0){
            free(temp->previous);
            temp = temp->next;
        }
        free(li->back);
    }
    li->front = li->back = (void*)(li->size = 0);
}

List* listCreate(){
    List *li = (List*)malloc(sizeof(List));
    li->front = li->back = (void*)(li->size = 0);
    return li;
}

void listFree(List *li){
    listClear(li);
    free(li);
}

void listValuesCallback(List *li, void(*f)(void*)){
    ListNode *node = li->front;
    while(node!=0){
        (*f)(node->value);
        node = node->next;
    }
}


void callbackFunction(void* value){
    printf("%i\n", value);
}

int main(){
    List *list = listCreate();
    int i;
    for(i=0; i<10; i++)
        listPushBack(list, (void*)i);
    for(i=0; i<5; i++)
        listPopBack(list);
    for(i = 1; i<5; i++)
        listPushFront(list, (void*)i);
    listValuesCallback(list, &callbackFunction);
    listFree(list);
    return 0;
}
