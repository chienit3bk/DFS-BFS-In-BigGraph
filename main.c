#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define FileName "C:\\Users\\admin\\Desktop\\Project1\\data\\roadNet-PA.txt"

typedef struct Node {
    int data;
    struct Node* next;
} node;

node* newNode(int data) {
    node* new_Node = (node*)malloc(sizeof(node));
    new_Node->data = data;
    new_Node->next = NULL;
    return new_Node;
}

typedef struct Stack {
    node *top;
} stack;

void InitS(stack *S) {
    S->top = NULL;
}

bool isEmptyS(stack *s) {
    return s->top == NULL;
}

void pushS(stack *s, int data) {
    node* new_node = newNode(data);
    new_node->next = s->top;
    s->top = new_node;
}

void popS(stack *s) {
    node* tmp = s->top;
    s->top = s->top->next;
    free(tmp);
}

node* top(stack *S) {
    return S->top;
}

typedef struct Queue {
    node *front, *rear;
} queue;

bool isEmptyQ(queue *Q) {
    return ((Q->front == NULL) || (Q->rear == NULL));
}

void InitQ(queue *Q) {
    Q->front = NULL;
    Q->rear = NULL;
}

void pushQ(queue *Q, int data) {
    node* new_Node = newNode(data);
    if(isEmptyQ(Q))
    {
        Q->front = new_Node;
        Q->rear = new_Node;
    }
    else
    {
        Q->rear->next = new_Node;
        Q->rear = Q->rear->next;
    }
}

node* front(queue *Q) {
    return Q->front;
}

void popQ(queue *Q) {
    node* tmp = Q->front;
    Q->front = Q->front->next;
    free(tmp);
}

typedef struct AdjList {
    node *head;
    int size;
} AdjList;

int sizeOfList() {
    int V = 0;
    int src, dest;
    FILE *fptr;
    fptr = fopen(FileName, "r");
    while(fscanf(fptr, "%d\t%d", &src, &dest) != EOF)
    {
        if(V < src) V = src;
    }
    V += 1;
    fclose(fptr);
    return V;
}

node* reverse(node* head) {
    node* prev = NULL;
    node* current = head;
    node* next = NULL;
    while (current != NULL)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    free(current);
    free(next);
    return prev;
}

void addEdge(AdjList* array, int src, int dest) {
    node* new_Node = newNode(dest);
    new_Node->next = array[src].head;
    array[src].head = new_Node;
}

AdjList* listNode() {
    int V = sizeOfList();
    int count_dinh = 0;
    int count_canh = 0;
    AdjList* array;
    array = (AdjList*)malloc(V * sizeof(AdjList));
    array->size = V;
    for(int i = 0; i < V; i++) {
        array[i].head = NULL;
    }
    FILE* fptr;
    int src, dest;
    fptr = fopen(FileName, "r");
    while(fscanf(fptr, "%d\t%d", &src, &dest) != EOF) {
        addEdge(array, src, dest);
        count_canh ++;
    }
    fclose(fptr);
    int W = 0;
    for(int i = 0; i < V; i++) {
        if(array[i].head != NULL) {
            array[i].head = reverse(array[i].head);
            W++;
        }
    }

    fclose(fptr);
    printf(FileName);
    printf("\nSo dinh: %d", W);
    printf("\nSo canh: %d", count_canh);
    return array;
}

void DFS(AdjList* array) {
    clock_t start_t, end_t;
    start_t = clock();
    int x = 0;
    int count = 0;
    bool visited[array->size + 1];
    for(int i = 0; i < array->size; i++) {
        visited[i] = false;
    }
    printf("\nDFS:");
    stack *S = (stack*)malloc(sizeof(stack));
    InitS(S);
    start_t = clock();
    for(int s = 0; s < array->size; s++) {
       if((!visited[s]) && (array[s].head != NULL)) {
            visited[s] = true;
//            printf("->%d", s);

            x++;
            if(x%100000 == 0) {
                printf("\n%d", x);
            }
            pushS(S, s);
            while(!isEmptyS(S)) {
                int u = top(S)->data;
                node *tmp = array[u].head;
                while(tmp != NULL) {
                    int v = tmp->data;
                    if(!visited[v]) {
 //                       printf("->%d", v);
                        x++;
                        if(x%100000 == 0) {
                            printf("\n%d", x);
                        }
                        pushS(S, v);
                        visited[v] = true;
                        break;
                    }
                    else {
                        tmp = tmp->next;
                        if(tmp == NULL) popS(S);
                    }
                }
                tmp = NULL;
            }
            count ++;
        }
    }
    end_t = clock();
    double total_t = (double)(end_t - start_t);
    printf("\nTime DFS: %.2f", total_t/ CLOCKS_PER_SEC);
    //printf("\n\nComponent Number Connected : %d", count);
}

void BFS(AdjList* array) {
    clock_t start_t, end_t;
    start_t = clock();
    int count = 0;
    int x = 0;
    bool visited[array->size + 1];
    for(int i = 0; i < array->size; i++) {
        visited[i] = false;
    }
    printf("\n\nBFS :");
    queue *Q = (queue*)malloc(sizeof(queue));
    InitQ(Q);
    for(int s = 0; s < array->size; s++) {
       if(!visited[s] && array[s].head != NULL) {
            queue *Q = (queue*)malloc(sizeof(queue));
            InitQ(Q);
            visited[s] = true;
            pushQ(Q, s);
            while(!isEmptyQ(Q)) {
                int u = front(Q)->data;
//                printf("->%d", u);
                x++;
                if(x%100000 == 0) {
                    printf("\n%d", x);
                }
                popQ(Q);

                node* tmp = array[u].head;
                while(tmp != NULL) {
                    int v = tmp->data;
                    if(!visited[v]) {
                    visited[v] = true;
                    pushQ(Q, v);
                    }
                    tmp = tmp->next;
                }
            }
            count ++;
       }
    }
    end_t = clock();
    double total_t = (double)(end_t - start_t);
    printf("\nTime BFS: %.2f", total_t/ CLOCKS_PER_SEC);
    printf("\ncomponent number connected : %d", count);
}

int main() {
    AdjList* array;
    array = listNode();
    DFS(array);
    BFS(array);
    return 0;
}
