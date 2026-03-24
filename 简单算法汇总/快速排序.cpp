#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 全局计数器
int comparison_count = 0;
int swap_count = 0;
int recursion_depth = 0;
int max_recursion_depth = 0;

// 交换函数
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
    swap_count++;
}

// 分区函数
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    
    for (int j = low; j <= high - 1; j++) {
        comparison_count++;
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// 快速排序主函数
void quickSort(int arr[], int low, int high, int current_depth) {
    recursion_depth++;
    if (current_depth > max_recursion_depth) {
        max_recursion_depth = current_depth;
    }
    
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1, current_depth + 1);
        quickSort(arr, pi + 1, high, current_depth + 1);
    }
    
    recursion_depth--;
}

// 打印数组
void printArray(int arr[], int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]");
}

// 检查数组是否有序
int isSorted(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

int main() {
    int n;
    
    printf("=== 快速排序算法测试程序 ===\n");
    
    printf("请输入数组元素个数: ");
    scanf("%d", &n);
    
    int* arr = (int*)malloc(n * sizeof(int));
    printf("请输入%d个整数: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    
    printf("\n原始数组: ");
    printArray(arr, n);
    
    // 重置计数器
    comparison_count = 0;
    swap_count = 0;
    max_recursion_depth = 0;
    
    clock_t start = clock();
    quickSort(arr, 0, n - 1, 1);
    clock_t end = clock();
    double time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("\n排序后数组: ");
    printArray(arr, n);
    
    printf("\n\n性能分析报告:\n");
    printf("==============\n");
    printf("排序验证: %s\n", isSorted(arr, n) ? "成功" : "失败");
    printf("执行时间: %.6f秒\n", time_used);
    printf("比较次数: %d\n", comparison_count);
    printf("交换次数: %d\n", swap_count);
    printf("最大递归深度: %d\n", max_recursion_depth);
    printf("理论时间复杂度: O(n log n) - O(n2)\n");
    printf("理论空间复杂度: O(log n) - O(n)\n");
    
    free(arr);
    return 0;
}
