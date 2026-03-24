#include <stdio.h>
#define MAX_VERTICES 100
// 打印邻接矩阵
void printAdjMatrix(int adjMatrix[MAX_VERTICES][MAX_VERTICES], int vertices) {
printf("邻接矩阵：\n");
for (int i = 0; i < vertices; i++) {
for (int j = 0; j < vertices; j++) {
printf("%d ", adjMatrix[i][j]);
}
printf("\n");
}
}
int main() {
int vertices, edges;
int adjMatrix[MAX_VERTICES][MAX_VERTICES];
int isDirected;
// 输入顶点数
printf("请输入顶点数：");
scanf("%d", &vertices);
// 输入图的类型（有向图或无向图）
printf("请输入图的类型（1 表示有向图，0 表示无向图）：");
scanf("%d", &isDirected);  
// 初始化邻接矩阵为0
for (int i = 0; i < vertices; i++) {
for (int j = 0; j < vertices; j++) {
adjMatrix[i][j] = 0;
}
}
// 输入边的数量
printf("请输入边的数量：");
scanf("%d", &edges);
// 输入边的信息
printf("请输入每条边的两个顶点（格式：顶点1 顶点2）：\n");
for (int i = 0; i < edges; i++) {
int u, v;
scanf("%d %d", &u, &v);    
// 如果是有向图，只设置 adjMatrix[u][v]
// 如果是无向图，设置 adjMatrix[u][v] 和 adjMatrix[v][u]
adjMatrix[u][v] = 1;
if (!isDirected) {
adjMatrix[v][u] = 1;  // 如果是无向图，设置对称位置
}
}
// 打印邻接矩阵
printAdjMatrix(adjMatrix, vertices);
return 0;
}


