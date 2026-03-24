#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 全局性能计数器
int comparison_count = 0;
int swap_count = 0;
int recursive_calls = 0;
int partition_calls = 0;

// 交换函数
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
    swap_count++;
}

// 分区函数
int partition(int arr[], int low, int high) {
    partition_calls++;
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

// 快速选择算法
int quickSelect(int arr[], int left, int right, int k) {
    recursive_calls++;
    
    if (left == right) {
        return arr[left];
    }
    
    int pivotIndex = partition(arr, left, right);
    int order = pivotIndex - left + 1;
    
    if (k == order) {
        return arr[pivotIndex];
    } else if (k < order) {
        return quickSelect(arr, left, pivotIndex - 1, k);
    } else {
        return quickSelect(arr, pivotIndex + 1, right, k - order);
    }
}

// 快速排序（用于对比验证）
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// 打印数组
void printArray(int arr[], int size) {
    printf("[");
    for (int i = 0; i < size && i < 20; i++) { // 限制输出长度
        printf("%d", arr[i]);
        if (i < size - 1 && i < 19) printf(", ");
    }
    if (size > 20) printf("...");
    printf("]");
}

int main() {
    int n, k;
    
    printf("=== 线性时间选择算法测试程序 ===\n");
    
    printf("请输入数组元素个数: ");
    scanf("%d", &n);
    
    int* arr = (int*)malloc(n * sizeof(int));
    int* arr_copy = (int*)malloc(n * sizeof(int));
    
    printf("请输入%d个整数: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
        arr_copy[i] = arr[i];
    }
    
    printf("请输入要查找的第k小元素(k从1开始): ");
    scanf("%d", &k);
    
    if (k < 1 || k > n) {
        printf("错误: k值必须在1到%d之间\n", n);
        free(arr);
        free(arr_copy);
        return 1;
    }
    
    printf("\n数组: ");
    printArray(arr, n);
    printf("\n查找第%d小的元素\n", k);
    
    // 重置计数器
    comparison_count = 0;
    swap_count = 0;
    recursive_calls = 0;
    partition_calls = 0;
    
    // 测试快速选择算法
    clock_t start1 = clock();
    int result = quickSelect(arr, 0, n - 1, k);
    clock_t end1 = clock();
    double time1 = ((double)(end1 - start1)) / CLOCKS_PER_SEC;
    
    // 测试排序选择方法（对比）
    clock_t start2 = clock();
    quickSort(arr_copy, 0, n - 1);
    int sorted_result = arr_copy[k - 1];
    clock_t end2 = clock();
    double time2 = ((double)(end2 - start2)) / CLOCKS_PER_SEC;
    
    printf("\n算法结果对比:\n");
    printf("================\n");
    printf("快速选择算法结果: %d\n", result);
    printf("排序验证结果: %d\n", sorted_result);
    printf("结果一致性: %s\n", result == sorted_result ? "正确" : "错误");
    
    printf("\n性能分析报告:\n");
    printf("==============\n");
    printf("快速选择算法:\n");
    printf("- 执行时间: %.6f秒\n", time1);
    printf("- 比较次数: %d\n", comparison_count);
    printf("- 交换次数: %d\n", swap_count);
    printf("- 递归调用次数: %d\n", recursive_calls);
    printf("- 分区调用次数: %d\n", partition_calls);
    
    printf("\n排序选择方法:\n");
    printf("- 执行时间: %.6f秒\n", time2);
    printf("- 性能提升: %.2fx\n", time2 / time1);
    
    printf("\n理论分析:\n");
    printf("- 平均时间复杂度: O(n)\n");
    printf("- 最坏时间复杂度: O(n2)\n");
    printf("- 空间复杂度: O(log n)\n");
    
    free(arr);
    free(arr_copy);
    return 0;
}
