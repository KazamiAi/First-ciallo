#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_N 100

void matrixChainOrder(int p[], int n, int m[MAX_N][MAX_N], int s[MAX_N][MAX_N]) {
    int i, j, k, l, q;
    
    // 初始化对角线
    for (i = 1; i <= n; i++) {
        m[i][i] = 0;
    }
    
    // l为链长
    for (l = 2; l <= n; l++) {
        for (i = 1; i <= n - l + 1; i++) {
            j = i + l - 1;
            m[i][j] = INT_MAX;
            
            for (k = i; k <= j - 1; k++) {
                q = m[i][k] + m[k+1][j] + p[i-1] * p[k] * p[j];
                if (q < m[i][j]) {
                    m[i][j] = q;
                    s[i][j] = k;
                }
            }
        }
    }
}

void printOptimalParens(int s[MAX_N][MAX_N], int i, int j) {
    if (i == j) {
        printf("A%d", i);
    } else {
        printf("(");
        printOptimalParens(s, i, s[i][j]);
        printOptimalParens(s, s[i][j] + 1, j);
        printf(")");
    }
}

void printCostTable(int m[MAX_N][MAX_N], int n) {
    printf("\n代价表m[i][j]:\n");
    printf("   ");
    for (int j = 1; j <= n; j++) {
        printf("%6d", j);
    }
    printf("\n");
    
    for (int i = 1; i <= n; i++) {
        printf("%2d ", i);
        for (int j = 1; j <= n; j++) {
            if (i <= j) {
                printf("%6d", m[i][j]);
            } else {
                printf("      ");
            }
        }
        printf("\n");
    }
}

int main() {
    int n;
    int p[MAX_N];
    int m[MAX_N][MAX_N], s[MAX_N][MAX_N];
    
    printf("=== 矩阵连乘问题求解 ===\n");
    
    printf("请输入矩阵个数: ");
    scanf("%d", &n);
    
    printf("请输入%d个矩阵的维度(共%d个数): ", n, n + 1);
    for (int i = 0; i <= n; i++) {
        scanf("%d", &p[i]);
    }
    
    matrixChainOrder(p, n, m, s);
    
    printf("\n计算结果:\n");
    printf("最小数乘次数: %d\n", m[1][n]);
    printf("最优括号化方案: ");
    printOptimalParens(s, 1, n);
    printf("\n");
    
    char choice;
    printf("\n是否显示代价表? (y/n): ");
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y') {
        printCostTable(m, n);
    }
    
    return 0;
}
