#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_N 100

typedef struct {
    int weight;
    int value;
    double density;
    int index;
} Item;

typedef struct Node {
    int level;
    int profit;
    int weight;
    double bound;
    int *solution;
} Node;

typedef struct {
    Node *nodes[MAX_N * MAX_N];
    int size;
} MaxHeap;

int n, capacity;
Item items[MAX_N];
int best_value = 0;
int best_solution[MAX_N];
int node_count = 0;

// 比较函数：按价值密度降序排序
int compare(const void *a, const void *b) {
    Item *item1 = (Item *)a;
    Item *item2 = (Item *)b;
    if (item1->density > item2->density) return -1;
    if (item1->density < item2->density) return 1;
    return 0;
}

// 计算上界函数
double bound(Node *u) {
    if (u->weight >= capacity) return 0;
    
    double bound_value = (double)u->profit;
    int remaining_capacity = capacity - u->weight;
    int j = u->level + 1;
    
    // 贪心装入剩余物品
    while (j < n && items[j].weight <= remaining_capacity) {
        remaining_capacity -= items[j].weight;
        bound_value += items[j].value;
        j++;
    }
    
    // 装入部分物品
    if (j < n) {
        bound_value += (double)items[j].value * remaining_capacity / items[j].weight;
    }
    
    return bound_value;
}

// 最大堆操作
void maxHeapPush(MaxHeap *heap, Node *node) {
    heap->nodes[heap->size] = node;
    int i = heap->size;
    heap->size++;
    
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap->nodes[parent]->bound >= heap->nodes[i]->bound) {
            break;
        }
        Node *temp = heap->nodes[parent];
        heap->nodes[parent] = heap->nodes[i];
        heap->nodes[i] = temp;
        i = parent;
    }
}

Node *maxHeapPop(MaxHeap *heap) {
    if (heap->size == 0) return NULL;
    
    Node *result = heap->nodes[0];
    heap->size--;
    heap->nodes[0] = heap->nodes[heap->size];
    
    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;
        
        if (left < heap->size && heap->nodes[left]->bound > heap->nodes[largest]->bound) {
            largest = left;
        }
        if (right < heap->size && heap->nodes[right]->bound > heap->nodes[largest]->bound) {
            largest = right;
        }
        
        if (largest != i) {
            Node *temp = heap->nodes[i];
            heap->nodes[i] = heap->nodes[largest];
            heap->nodes[largest] = temp;
            i = largest;
        } else {
            break;
        }
    }
    
    return result;
}

// 分支限界法
void branchBoundKnapsack() {
    MaxHeap heap;
    heap.size = 0;
    
    // 创建根节点
    Node *u = (Node *)malloc(sizeof(Node));
    u->level = -1;
    u->profit = 0;
    u->weight = 0;
    u->solution = (int *)calloc(n, sizeof(int));
    u->bound = bound(u);
    maxHeapPush(&heap, u);
    
    while (heap.size > 0) {
        node_count++;
        Node *u = maxHeapPop(&heap);
        
        if (u->bound > best_value) {
            // 生成左子节点（选择物品）
            if (u->level + 1 < n) {
                Node *v = (Node *)malloc(sizeof(Node));
                v->level = u->level + 1;
                v->weight = u->weight + items[v->level].weight;
                v->profit = u->profit + items[v->level].value;
                v->solution = (int *)malloc(n * sizeof(int));
                for (int i = 0; i < n; i++) {
                    v->solution[i] = u->solution[i];
                }
                v->solution[items[v->level].index] = 1;
                
                if (v->weight <= capacity && v->profit > best_value) {
                    best_value = v->profit;
                    for (int i = 0; i < n; i++) {
                        best_solution[i] = v->solution[i];
                    }
                }
                
                v->bound = bound(v);
                if (v->bound > best_value) {
                    maxHeapPush(&heap, v);
                } else {
                    free(v->solution);
                    free(v);
                }
            }
            
            // 生成右子节点（不选择物品）
            if (u->level + 1 < n) {
                Node *v = (Node *)malloc(sizeof(Node));
                v->level = u->level + 1;
                v->weight = u->weight;
                v->profit = u->profit;
                v->solution = (int *)malloc(n * sizeof(int));
                for (int i = 0; i < n; i++) {
                    v->solution[i] = u->solution[i];
                }
                v->solution[items[v->level].index] = 0;
                
                v->bound = bound(v);
                if (v->bound > best_value) {
                    maxHeapPush(&heap, v);
                } else {
                    free(v->solution);
                    free(v);
                }
            }
        }
        
        free(u->solution);
        free(u);
    }
}

int main() {
    printf("=== 0-1背包问题求解（分支限界法） ===\n");
    
    printf("请输入物品个数: ");
    scanf("%d", &n);
    
    printf("请输入背包容量: ");
    scanf("%d", &capacity);
    
    printf("请依次输入每个物品的重量和价值:\n");
    for (int i = 0; i < n; i++) {
        printf("物品%d: ", i + 1);
        scanf("%d %d", &items[i].weight, &items[i].value);
        items[i].density = (double)items[i].value / items[i].weight;
        items[i].index = i;
        best_solution[i] = 0;
    }
    
    // 按价值密度排序
    qsort(items, n, sizeof(Item), compare);
    
    printf("\n按价值密度排序后的物品:\n");
    printf("ID\t重量\t价值\t价值密度\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%.2f\n", 
               items[i].index + 1, items[i].weight, items[i].value, items[i].density);
    }
    
    printf("\n开始分支限界搜索...\n");
    branchBoundKnapsack();
    
    printf("\n计算结果:\n");
    printf("最优价值: %d\n", best_value);
    
    int total_weight = 0;
    printf("最优解向量: [");
    for (int i = 0; i < n; i++) {
        printf("%d", best_solution[i]);
        if (i < n - 1) printf(", ");
        if (best_solution[i] == 1) {
            total_weight += items[i].weight;
        }
    }
    printf("]\n");
    
    printf("总重量: %d\n", total_weight);
    
    printf("\n选择的物品:\n");
    printf("ID\t重量\t价值\n");
    for (int i = 0; i < n; i++) {
        if (best_solution[i] == 1) {
            printf("%d\t%d\t%d\n", i + 1, items[i].weight, items[i].value);
        }
    }
    
    printf("\n搜索统计:\n");
    printf("扩展节点数: %d\n", node_count);
    
    return 0;
}
