#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int start;
    int finish;
    int id;
} Activity;

// 比较函数：按结束时间升序排序
int compare(const void *a, const void *b) {
    Activity *act1 = (Activity *)a;
    Activity *act2 = (Activity *)b;
    return act1->finish - act2->finish;
}

void activitySelection(Activity activities[], int n) {
    // 按结束时间排序
    qsort(activities, n, sizeof(Activity), compare);
    
    printf("\n按结束时间排序后的活动:\n");
    printf("ID\t开始\t结束\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\n", activities[i].id, activities[i].start, activities[i].finish);
    }
    
    printf("\n选择的活动:\n");
    printf("ID\t开始\t结束\n");
    
    // 第一个活动总是被选择
    int i = 0;
    printf("%d\t%d\t%d\n", activities[i].id, activities[i].start, activities[i].finish);
    int selected_count = 1;
    
    // 考虑剩余活动
    for (int j = 1; j < n; j++) {
        // 如果此活动的开始时间大于或等于上一个选择活动的结束时间，则选择它
        if (activities[j].start >= activities[i].finish) {
            printf("%d\t%d\t%d\n", activities[j].id, activities[j].start, activities[j].finish);
            i = j;
            selected_count++;
        }
    }
    
    printf("\n总共选择了 %d 个活动\n", selected_count);
}

int main() {
    int n;
    Activity activities[100];
    
    printf("=== 活动安排问题求解 ===\n");
    
    printf("请输入活动个数: ");
    scanf("%d", &n);
    
    printf("请依次输入每个活动的开始时间和结束时间:\n");
    for (int i = 0; i < n; i++) {
        printf("活动%d: ", i + 1);
        scanf("%d %d", &activities[i].start, &activities[i].finish);
        activities[i].id = i + 1;
        
        // 验证输入有效性
        if (activities[i].start < 0 || activities[i].finish < 0 || 
            activities[i].start >= activities[i].finish) {
            printf("错误: 开始时间必须小于结束时间，且都为非负数\n");
            return 1;
        }
    }
    
    printf("\n所有活动:\n");
    printf("ID\t开始\t结束\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\n", activities[i].id, activities[i].start, activities[i].finish);
    }
    
    activitySelection(activities, n);
    
    return 0;
}
