#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 100

int LCS(char X[], char Y[], int m, int n, char result[]) {
    int c[MAX_LEN][MAX_LEN];
    char b[MAX_LEN][MAX_LEN];
    int i, j;
    
    // 构建LCS表
    for (i = 0; i <= m; i++) {
        for (j = 0; j <= n; j++) {
            if (i == 0 || j == 0) {
                c[i][j] = 0;
                b[i][j] = '0';
            } else if (X[i - 1] == Y[j - 1]) {
                c[i][j] = c[i - 1][j - 1] + 1;
                b[i][j] = '\\';  // 左上
            } else if (c[i - 1][j] >= c[i][j - 1]) {
                c[i][j] = c[i - 1][j];
                b[i][j] = '|';   // 上
            } else {
                c[i][j] = c[i][j - 1];
                b[i][j] = '-';   // 左
            }
        }
    }
    
    // 回溯构造LCS
    int index = c[m][n];
    result[index] = '\0';
    
    i = m, j = n;
    while (i > 0 && j > 0) {
        if (b[i][j] == '\\') {
            result[index - 1] = X[i - 1];
            i--; j--; index--;
        } else if (b[i][j] == '|') {
            i--;
        } else {
            j--;
        }
    }
    
    return c[m][n];
}

void printLCSTable(char X[], char Y[], int m, int n) {
    int c[MAX_LEN][MAX_LEN];
    
    // 构建LCS表用于显示
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0 || j == 0) {
                c[i][j] = 0;
            } else if (X[i - 1] == Y[j - 1]) {
                c[i][j] = c[i - 1][j - 1] + 1;
            } else {
                c[i][j] = (c[i - 1][j] > c[i][j - 1]) ? c[i - 1][j] : c[i][j - 1];
            }
        }
    }
    
    printf("\nLCS长度表:\n   ");
    for (int j = 0; j <= n; j++) {
        if (j == 0) printf("  ");
        else printf(" %c", Y[j-1]);
    }
    printf("\n");
    
    for (int i = 0; i <= m; i++) {
        if (i == 0) printf("  ");
        else printf("%c ", X[i-1]);
        for (int j = 0; j <= n; j++) {
            printf("%2d", c[i][j]);
        }
        printf("\n");
    }
}

int main() {
    char X[MAX_LEN], Y[MAX_LEN];
    char result[MAX_LEN];
    
    printf("=== 最长公共子序列问题求解 ===\n");
    
    printf("请输入第一个序列X: ");
    scanf("%s", X);
    printf("请输入第二个序列Y: ");
    scanf("%s", Y);
    
    int m = strlen(X);
    int n = strlen(Y);
    
    int lcs_length = LCS(X, Y, m, n, result);
    
    printf("\n计算结果:\n");
    printf("序列X: %s (长度: %d)\n", X, m);
    printf("序列Y: %s (长度: %d)\n", Y, n);
    printf("最长公共子序列长度: %d\n", lcs_length);
    printf("最长公共子序列: %s\n", result);
    
    char choice;
    printf("\n是否显示LCS长度表? (y/n): ");
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y') {
        printLCSTable(X, Y, m, n);
    }
    
    return 0;
}
