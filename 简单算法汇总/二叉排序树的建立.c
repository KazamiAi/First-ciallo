#include <stdio.h>
#include <stdlib.h>
// 定义二叉树节点结构
typedef struct TreeNode {
int value;             // 节点的值
struct TreeNode* left; // 左子树
struct TreeNode* right;// 右子树
} TreeNode;
// 创建新节点
TreeNode* createNode(int value) {
TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
newNode->value = value;
newNode->left = newNode->right = NULL;
return newNode;
}
// 先序遍历：根 -> 左 -> 右
void preorderTraversal(TreeNode* root) {if (root != NULL) {
printf("%d ", root->value); // 访问根节点
preorderTraversal(root->left); // 递归左子树
preorderTraversal(root->right); // 递归右子树
}
}
// 中序遍历：左 -> 根 -> 右
void inorderTraversal(TreeNode* root) {
if (root != NULL) {
inorderTraversal(root->left); // 递归左子树
printf("%d ", root->value); // 访问根节点
inorderTraversal(root->right); // 递归右子树
}
}
// 后序遍历：左 -> 右 -> 根
void postorderTraversal(TreeNode* root) {
if (root != NULL) {
postorderTraversal(root->left); // 递归左子树
postorderTraversal(root->right); // 递归右子树
printf("%d ", root->value); // 访问根节点
}
}
// 计算二叉树深度
int getTreeDepth(TreeNode* root) {
if (root == NULL) {
return 0; // 空树的深度为0
} else {
// 递归计算左子树和右子树的深度，取最大值加1
int leftDepth = getTreeDepth(root->left);
int rightDepth = getTreeDepth(root->right);
return (leftDepth > rightDepth ? leftDepth : rightDepth) + 1;
}
}
// 向二叉树插入节点
TreeNode* insert(TreeNode* root, int value) {
// 如果树为空，创建新节点
if (root == NULL) {
return createNode(value);
}
// 否则递归插入到左子树或右子树
if (value < root->value) {
root->left = insert(root->left, value);
} else {
root->right = insert(root->right, value);
}
return root;
}
// 主函数
int main() {
TreeNode* root = NULL;
int n, value;
// 输入要插入的节点个数
printf("请输入要插入节点的个数: ");
scanf("%d", &n);
printf("请输入 %d 个节点的值:\n", n);
for (int i = 0; i < n; i++) {
scanf("%d", &value);
root = insert(root, value);
}
// 输出遍历结果
printf("先序遍历结果: ");
preorderTraversal(root);
printf("\n");
printf("中序遍历结果: ");
inorderTraversal(root);
printf("\n");
printf("后序遍历结果: ");
postorderTraversal(root);
printf("\n");
// 计算并输出树的深度
int depth = getTreeDepth(root);
printf("二叉树的深度为: %d\n", depth);
return 0;
}

