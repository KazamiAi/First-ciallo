#include <stdio.h>
#include <stdlib.h>

// 二分搜索算法
int binarySearch(int arr[], int left, int right, int target) {
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;
        }
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

// 打印数组
void printArray(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]");
}

int main() {
    int choice, n, target;
    
    printf("=== 二分搜索算法测试程序 ===\n");
    
    printf("请输入数组元素个数: ");
    scanf("%d", &n);
    
    int* arr = (int*)malloc(n * sizeof(int));
    printf("请输入%d个有序整数(从小到大): ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    
    printf("请输入要查找的目标值: ");
    scanf("%d", &target);
    
    printf("\n数组: ");
    printArray(arr, n);
    printf("\n目标值: %d\n", target);
    
    int result = binarySearch(arr, 0, n - 1, target);
    
    if (result != -1) {
        printf("结果: 找到元素，索引位置: %d\n", result);
    } else {
        printf("结果: 未找到元素\n");
    }
    
    free(arr);
    return 0;
}
