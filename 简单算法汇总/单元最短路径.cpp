#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_N 100
#define INF INT_MAX

typedef struct {
    int vertex;
    int distance;
} PathNode;

typedef struct {
    PathNode *nodes[MAX_N * MAX_N];
    int size;
} MinHeap;

int n;
int graph[MAX_N][MAX_N];
int dist[MAX_N];
int visited[MAX_N];
int previous[MAX_N];

// 最小堆操作
void minHeapPush(MinHeap *heap, PathNode *node) {
    heap->nodes[heap->size] = node;
    int i = heap->size;
    heap->size++;
    
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap->nodes[parent]->distance <= heap->nodes[i]->distance) {
            break;
        }
        PathNode *temp = heap->nodes[parent];
        heap->nodes[parent] = heap->nodes[i];
        heap->nodes[i] = temp;
        i = parent;
    }
}

PathNode *minHeapPop(MinHeap *heap) {
    if (heap->size == 0) return NULL;
    
    PathNode *result = heap->nodes[0];
    heap->size--;
    heap->nodes[0] = heap->nodes[heap->size];
    
    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
        
        if (left < heap->size && heap->nodes[left]->distance < heap->nodes[smallest]->distance) {
            smallest = left;
        }
        if (right < heap->size && heap->nodes[right]->distance < heap->nodes[smallest]->distance) {
            smallest = right;
        }
        
        if (smallest != i) {
            PathNode *temp = heap->nodes[i];
            heap->nodes[i] = heap->nodes[smallest];
            heap->nodes[smallest] = temp;
            i = smallest;
        } else {
            break;
        }
    }
    
    return result;
}

// Dijkstra算法
void dijkstra(int src) {
    MinHeap heap;
    heap.size = 0;
    
    // 初始化
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        visited[i] = 0;
        previous[i] = -1;
    }
    dist[src] = 0;
    
    // 源点入队
    PathNode *node = (PathNode *)malloc(sizeof(PathNode));
    node->vertex = src;
    node->distance = 0;
    minHeapPush(&heap, node);
    
    while (heap.size > 0) {
        PathNode *u_node = minHeapPop(&heap);
        int u = u_node->vertex;
        
        // 如果当前节点的距离已经大于已知最短距离，跳过
        if (u_node->distance > dist[u]) {
            free(u_node);
            continue;
        }
        
        visited[u] = 1;
        
        // 遍历所有邻接点
        for (int v = 0; v < n; v++) {
            if (graph[u][v] != INF && graph[u][v] != 0) {
                int new_dist = dist[u] + graph[u][v];
                if (new_dist < dist[v]) {
                    dist[v] = new_dist;
                    previous[v] = u;
                    
                    PathNode *v_node = (PathNode *)malloc(sizeof(PathNode));
                    v_node->vertex = v;
                    v_node->distance = new_dist;
                    minHeapPush(&heap, v_node);
                }
            }
        }
        free(u_node);
    }
}

// 打印最短路径
void printPath(int dest) {
    if (previous[dest] != -1) {
        printPath(previous[dest]);
        printf(" -> ");
    }
    printf("%d", dest);
}

// 打印邻接矩阵
void printGraph() {
    printf("\n图的邻接矩阵:\n   ");
    for (int i = 0; i < n; i++) {
        printf("%4d", i);
    }
    printf("\n");
    
    for (int i = 0; i < n; i++) {
        printf("%2d ", i);
        for (int j = 0; j < n; j++) {
            if (graph[i][j] == INF) {
                printf("   ∞");
            } else {
                printf("%4d", graph[i][j]);
            }
        }
        printf("\n");
    }
}

int main() {
    int src;
    
    printf("=== 单源最短路径问题求解（Dijkstra算法） ===\n");
    
    printf("请输入顶点个数: ");
    scanf("%d", &n);
    
    printf("请选择图的输入方式:\n");
    printf("1. 手动输入邻接矩阵\n");
    printf("2. 输入边信息\n");
    printf("选择: ");
    
    int choice;
    scanf("%d", &choice);
    
    // 初始化图
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                graph[i][j] = 0;
            } else {
                graph[i][j] = INF;
            }
        }
    }
    
    if (choice == 1) {
        printf("请输入%d×%d的邻接矩阵(用-1表示不可达):\n", n, n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int weight;
                scanf("%d", &weight);
                if (weight == -1) {
                    graph[i][j] = INF;
                } else {
                    graph[i][j] = weight;
                }
            }
        }
    } else {
        int edges;
        printf("请输入边数: ");
        scanf("%d", &edges);
        
        printf("请依次输入每条边的起点、终点和权重:\n");
        for (int i = 0; i < edges; i++) {
            int u, v, w;
            scanf("%d %d %d", &u, &v, &w);
            graph[u][v] = w;
            // 如果是无向图，取消下面注释
            // graph[v][u] = w;
        }
    }
    
    printf("请输入源点编号(0-%d): ", n - 1);
    scanf("%d", &src);
    
    printGraph();
    
    dijkstra(src);
    
    printf("\n从顶点%d到各顶点的最短路径:\n", src);
    printf("目标顶点\t最短距离\t路径\n");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < n; i++) {
        if (dist[i] == INF) {
            printf("%d\t\t不可达\t\t无路径\n", i);
        } else {
            printf("%d\t\t%d\t\t", i, dist[i]);
            printPath(i);
            printf("\n");
        }
    }
    
    return 0;
}
