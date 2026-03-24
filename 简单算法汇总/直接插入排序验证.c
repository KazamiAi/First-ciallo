#include <stdio.h>
void insertionSort(int arr[], int n) {
int i, key, j;
// 从第二个元素开始，因为第一个元素已经默认是有序的
for (i = 1; i < n; i++) {
key = arr[i]; // 当前元素
j = i - 1;
// 将大于 key 的元素向右移动一个位置
while (j >= 0 && arr[j] > key) {
arr[j + 1] = arr[j];
j = j - 1;
} 
// 将 key 放入正确的位置
arr[j + 1] = key;
}
}
// 用于打印数组的函数
void printArray(int arr[], int size) {
int i;
for (i = 0; i < size; i++) {
printf("%d ", arr[i]);
}
printf("\n");
}
int main() {
int n;
// 用户输入数组的长度
printf("请输入数组的长度: ");
scanf("%d", &n);
// 创建数组并获取用户输入的元素
int arr[n];
printf("请输入 %d 个整数:\n", n);
for (int i = 0; i < n; i++) {
scanf("%d", &arr[i]);
}
printf("原始数组:\n");
printArray(arr, n);
// 调用直接插入排序函数
insertionSort(arr, n);
printf("排序后的数组:\n");
printArray(arr, n);
return 0;
}


