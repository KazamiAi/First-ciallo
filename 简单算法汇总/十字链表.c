#include <stdio.h>
#include <stdlib.h>
// 十字链表节点结构
typedef struct CrossNode {
int row, col, value;
struct CrossNode *right, *down;
} CrossNode;
// 十字链表结构
typedef struct {
CrossNode **rowHeader, **colHeader;
int rows, cols;
} CrossList;
// 初始化十字链表
void initCrossList(CrossList *list, int rows, int cols) {
list->rows = rows;
list->cols = cols;
list->rowHeader = (CrossNode **)calloc(rows + 1, sizeof(CrossNode *)); // 行头指针
list->colHeader = (CrossNode **)calloc(cols + 1, sizeof(CrossNode *)); // 列头指针
}
// 创建节点
CrossNode* createNode(int row, int col, int value) {
CrossNode *newNode = (CrossNode *)malloc(sizeof(CrossNode));
newNode->row = row;
newNode->col = col;
newNode->value = value;
newNode->right = newNode->down = NULL;
return newNode;
}
// 插入节点
void insertNode(CrossList *list, int row, int col, int value) {
if (value == 0) return;  // 忽略值为0的元素    
CrossNode *newNode = createNode(row, col, value);
// 行链表插入
CrossNode *rowPtr = list->rowHeader[row];
if (rowPtr == NULL) {
list->rowHeader[row] = newNode;
} else {
while (rowPtr->right != NULL && rowPtr->right->col < col) {
rowPtr = rowPtr->right;
}
newNode->right = rowPtr->right;
rowPtr->right = newNode;
}
// 列链表插入
CrossNode *colPtr = list->colHeader[col];
if (colPtr == NULL) {
list->colHeader[col] = newNode;
} else {
while (colPtr->down != NULL && colPtr->down->row < row) {
colPtr = colPtr->down;
}
newNode->down = colPtr->down;
colPtr->down = newNode;
}
}
// 遍历十字链表并打印所有非零元素
void traverseCrossList(CrossList *list) {
printf("十字链表中的非零元素：\n");
for (int i = 1; i <= list->rows; i++) {
CrossNode *rowPtr = list->rowHeader[i];
while (rowPtr != NULL) {
printf("行 %d, 列 %d: %d\n", rowPtr->row, rowPtr->col, rowPtr->value);
rowPtr = rowPtr->right;
}
}
}
// 释放内存
void freeCrossList(CrossList *list) {
for (int i = 1; i <= list->rows; i++) {
CrossNode *rowPtr = list->rowHeader[i];
while (rowPtr != NULL) {
CrossNode *temp = rowPtr;
rowPtr = rowPtr->right;
free(temp);
}
}
free(list->rowHeader);
free(list->colHeader);
}
int main() {
CrossList list;
int rows, cols, nonZeroCount;
// 输入矩阵尺寸和非零元素数量
printf("请输入矩阵行数和列数：");
scanf("%d %d", &rows, &cols);
initCrossList(&list, rows, cols); 
printf("请输入非零元素的个数：");
scanf("%d", &nonZeroCount);
// 输入非零元素
for (int i = 0; i < nonZeroCount; i++) {
int row, col, value;
printf("请输入第 %d 个非零元素的行、列和值：", i + 1);
scanf("%d %d %d", &row, &col, &value);
insertNode(&list, row, col, value);
}
// 输出非零元素
traverseCrossList(&list);
// 释放内存
freeCrossList(&list);
return 0;
}


