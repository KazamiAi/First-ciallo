#include <stdio.h>
#include <stdlib.h>
typedef int ElementType;  // 栈元素类型
//链栈的节点
typedef struct Node {
    ElementType data;
    struct Node *next;
} Node;
// 链栈结构
typedef struct {
    Node *top;
} LinkStack;
// 初始化栈
void InitStack(LinkStack *stack) {
    stack->top = NULL;
}
// 判断栈是否为空
int IsEmpty(LinkStack *stack) {
    return stack->top == NULL;
}
// 入栈
void Push(LinkStack *stack, ElementType value) {
Node *newNode = (Node *)malloc(sizeof(Node));
newNode->data = value;
newNode->next = stack->top;
stack->top = newNode;
}
// 出栈
int Pop(LinkStack *stack, ElementType *value) {
if (IsEmpty(stack)) return 0;
Node *temp = stack->top;
*value = temp->data;
stack->top = temp->next;
free(temp);
return 1;
}
// 取栈顶元素
int Top(LinkStack *stack, ElementType *value) {
if (IsEmpty(stack)) return 0;
*value = stack->top->data;
return 1;
}
// 遍历栈
void Traverse(LinkStack *stack) {
Node *current = stack->top;
if (current == NULL) {
printf("栈为空！\n");
return;
}
printf("栈中的元素为：");
while (current) {
printf("%d ", current->data);
current = current->next;
}
printf("\n");
}
// 主程序
int main() {
LinkStack stack;
InitStack(&stack);
int choice, value;
char buffer[100];
while (1) {
// 显示菜单
printf("\n栈操作菜单：\n");
printf("1. 入栈\n");
printf("2. 出栈\n");
printf("3. 查看栈顶\n");
printf("4. 遍历栈\n");
printf("5. 退出\n");
printf("请输入你的选择（1-5）：");
scanf("%d", &choice);
getchar();  // 清空输入缓冲区
switch (choice) {
case 1:
printf("请输入要入栈的值（多个值用空格分隔，以回车结束）：");
fgets(buffer, sizeof(buffer), stdin);  // 允许输入多个值
char *token = strtok(buffer, " ");
while (token != NULL) {
value = atoi(token);  // 将输入的字符串转换为整数
Push(&stack, value);
token = strtok(NULL, " ");  // 获取下一个值
}
printf("已将多个值入栈。\n");
break;
case 2:
if (Pop(&stack, &value)) 
printf("已将 %d 出栈。\n", value);
else 
printf("栈为空，无法出栈！\n");
break;
case 3:
if (Top(&stack, &value)) 
printf("栈顶元素为：%d\n", value);
else 
printf("栈为空！\n");
break;
case 4:
Traverse(&stack);
break;
case 5:
printf("程序结束。\n");
return 0;
default:
printf("无效的选择，请输入1-5之间的数字！\n");
}
}
}
