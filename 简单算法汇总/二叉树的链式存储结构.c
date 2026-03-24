#include <stdio.h>
#include <stdlib.h>
// 二叉树节点结构体定义
typedef struct TreeNode {
int data;               // 存储节点数据
struct TreeNode* left;  // 指向左子节点的指针
struct TreeNode* right; // 指向右子节点的指针
} TreeNode;
//创建新节点
TreeNode* createNode(int data) {
TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
newNode->data = data;
newNode->left = NULL;
newNode->right = NULL;
return newNode;
}
// 插入节点，使用递归方式
TreeNode* insert(TreeNode* root, int data) {
if (root == NULL) {
return createNode(data);
}
// 假设数据小的放左子树，大的放右子树
if (data < root->data) {
root->left = insert(root->left, data);
} else {
root->right = insert(root->right, data);
}
return root;
}
// 前序遍历
void preOrder(TreeNode* root) {
if (root == NULL) return;
printf("%d ", root->data);
preOrder(root->left);
preOrder(root->right);
}
// 中序遍历
void inOrder(TreeNode* root) {
if (root == NULL) return;
inOrder(root->left);
printf("%d ", root->data);
inOrder(root->right);
}
// 后序遍历
void postOrder(TreeNode* root) {
if (root == NULL) return;
postOrder(root->left);
postOrder(root->right);
printf("%d ", root->data);
}
// 销毁树
void destroyTree(TreeNode* root) {
if (root == NULL) return;
destroyTree(root->left);
destroyTree(root->right);
free(root);
}
int main() {
TreeNode* root = NULL;
int n, value; 
printf("请输入二叉树节点的个数: ");
scanf("%d", &n);
printf("请输入二叉树节点的值: ");
for (int i = 0; i < n; i++) {
scanf("%d", &value);
root = insert(root, value);
}
printf("前序遍历: ");
preOrder(root);
printf("\n");
printf("中序遍历: ");
inOrder(root);
printf("\n");
printf("后序遍历: ");
postOrder(root);
printf("\n");
destroyTree(root);
return 0;
}



