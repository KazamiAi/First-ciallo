#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(_WIN32) || defined(_WIN64)
  #include <windows.h>
  #define SLEEP_MS(ms) Sleep(ms)
#else
  #include <unistd.h>
  #define SLEEP_MS(ms) usleep((ms)*1000)
#endif
#define COLOR_HIGHLIGHT "\x1b[33;1m" // 黄色加粗
#define COLOR_STEP "\x1b[31;1m"      // 红色加粗
#define MAX_NODES 300
#define MAX_NAME_LEN 64
#define MAX_NEI 64
#define MAX_EDGES 1024
#define COLOR_RESET "\x1b[0m"
#define DEFAULT_ANIM_MS 800

typedef struct {
char name[MAX_NAME_LEN];
} Node;//人名 

typedef struct {
int to;               // 目标节点索引
char label[48];       // 关系标签
} Neighbor;//人物关系 ，有向 

typedef struct {
Neighbor nei[MAX_NEI];
int count;
} AdjListEntry;//邻接表 

typedef struct { 
int a,b; 
char label[48];
} Edge;//记录边 

static Node nodes[MAX_NODES];//人物数据库 
static int node_count = 0;//当前人物数量 
static AdjListEntry adj[MAX_NODES];//关系网络邻接表 
static Edge edges[MAX_EDGES];//边记录 
static int edge_count = 0;//边总数 

// 添加节点 ，不存在则返回节点索引 
int add_node(const char *name) {
for (int i = 0; i < node_count; ++i) {
if (strcmp(nodes[i].name, name) == 0) return i;//查重，避免重复 
}
if (node_count >= MAX_NODES) {
fprintf(stderr, "节点数超过限制\n");
exit(1);//容量检查，安全拷贝 
}
strncpy(nodes[node_count].name, name, MAX_NAME_LEN-1);
nodes[node_count].name[MAX_NAME_LEN-1] = '\0';
adj[node_count].count = 0;
return node_count++;
}
//构建关系网络 
// 添加无向边(a<->b) 带标签;为dot导出存储边 
void add_edge_by_name(const char *a, const char *b, const char *label) {
int ia = add_node(a);
int ib = add_node(b);
//添加a到b
if (adj[ia].count < MAX_NEI) {
adj[ia].nei[adj[ia].count].to = ib;
strncpy(adj[ia].nei[adj[ia].count].label, label, 47);
adj[ia].nei[adj[ia].count].label[47] = '\0';
adj[ia].count++;
}
// 添加b到a反向关系 
if (adj[ib].count < MAX_NEI) {
adj[ib].nei[adj[ib].count].to = ia;
strncpy(adj[ib].nei[adj[ib].count].label, label, 47);
adj[ib].nei[adj[ib].count].label[47] = '\0';
adj[ib].count++;
    }
// 记录边(ia < ib) 
if (edge_count < MAX_EDGES && ia < ib) {
edges[edge_count].a = ia; edges[edge_count].b = ib;
strncpy(edges[edge_count].label, label, 47);
edges[edge_count].label[47] = '\0';
edge_count++;
}
}

// 通过姓名查找节点索引，找不到返回-1
int find_node(const char *name) {
for (int i = 0; i < node_count; ++i) if (strcmp(nodes[i].name, name) == 0) return i;
return -1;
}

// BFS最短路径搜索：从s到t，out_path接收节点索引，out_labels接收每一步的关系标签
//  找到返回路径长度(>=1)，否则返回0
int bfs_shortest_path(int s, int t, int out_path[], char out_labels[][48]) {
if (s < 0 || t < 0) return 0;
int prev[MAX_NODES];   // 前驱节点数组
char prev_label[MAX_NODES][48];// 前驱关系标签
// 初始化前驱数组
for (int i = 0; i < MAX_NODES; ++i) { 
prev[i] = -1; 
prev_label[i][0] = '\0';
}
int q[MAX_NODES], head = 0, tail = 0;// BFS队列
q[tail++] = s;
prev[s] = s; // 标记起始节点已访问
// BFS遍历
while (head < tail) {
int cur = q[head++];
if (cur == t) break; // 找到目标节点
 // 遍历当前节点的所有邻居
for (int i = 0; i < adj[cur].count; ++i) {
Neighbor nb = adj[cur].nei[i];
if (prev[nb.to] == -1) {
prev[nb.to] = cur;
strncpy(prev_label[nb.to], nb.label, 47);
prev_label[nb.to][47] = '\0';
q[tail++] = nb.to;
}
}
}
 // 如果目标节点不可达
if (prev[t] == -1) return 0;
int stack[MAX_NODES], sp = 0;
int cur = t;
while (!(cur == s && prev[cur] == s)) {
stack[sp++] = cur;
cur = prev[cur];
    }
stack[sp++] = s;
int len = sp;
 // 反转栈得到正序路径
for (int i = 0; i < len; ++i) {
out_path[i] = stack[sp - 1 - i];
}
for (int i = 0; i < len - 1; ++i) {
int from = out_path[i], to = out_path[i+1];
// find label from 'from' to 'to'
    char lab[48] = "?";// 默认标签
for (int k = 0; k < adj[from].count; ++k) {
if (adj[from].nei[k].to == to) {
strncpy(lab, adj[from].nei[k].label, 47); lab[47] = '\0';
break;
}
}
strncpy(out_labels[i], lab, 47); out_labels[i][47] = '\0';
}
out_labels[len-1][0] = '\0';
return len;
}

// 打印图结构摘要
void print_graph_summary() {
printf("--- 当前图谱（%d 人）---\n", node_count);
for (int i = 0; i < node_count; ++i) {
printf("%2d: %s -> ", i, nodes[i].name);
for (int j = 0; j < adj[i].count; ++j) {
printf("[%s:%s] ", nodes[adj[i].nei[j].to].name, adj[i].nei[j].label);
}
printf("\n");
}
printf("------------------------\n");
}
// 导出DOT格式文件
void export_dot(const char *filename) {
FILE *f = fopen(filename, "w");
if (!f) { perror("打开 DOT 文件失败"); return; }
fprintf(f, "graph JiaFamily {\n");
fprintf(f, "  node [shape=ellipse, style=filled, fillcolor=white];\n");
// 输出所有节点
for (int i = 0; i < node_count; ++i) {
// 转义引号字符
char buf[MAX_NAME_LEN*2]; int bi=0;
for (int j=0; nodes[i].name[j] && bi < (int)sizeof(buf)-1; ++j) {
if (nodes[i].name[j] == '\"') { buf[bi++] = '\\\\'; buf[bi++] = '\"'; }
else buf[bi++] = nodes[i].name[j];
}
buf[bi] = '\0';
fprintf(f, "  n%d [label=\"%s\"];\n", i, buf);
}
// 输出所有边
for (int e = 0; e < edge_count; ++e) {
// 转义标签中的引号
char lbl[64]; int bi=0;
for (int j=0; edges[e].label[j] && bi < (int)sizeof(lbl)-1; ++j) {
if (edges[e].label[j] == '\"') { lbl[bi++]='\\\\'; lbl[bi++]='\"'; }
else lbl[bi++] = edges[e].label[j];
}
lbl[bi] = '\0';
fprintf(f, "  n%d -- n%d [label=\"%s\"];\n", edges[e].a, edges[e].b, lbl);
}
fprintf(f, "}\n");
fclose(f);
printf("已导出 DOT 文件：%s（可用 graphviz 渲染）\n", filename);
}

// 路径动画演示：自动播放或按步推进
void animate_path(int path[], char labels[][48], int len, int ms_delay, int step_by_enter) {
if (len <= 0) return;
printf("\n开始逐步演示（%s，每步间隔 %dms）...\n",
step_by_enter ? "按回车推进" : "自动播放", ms_delay);
for (int step = 0; step < len; ++step) {
// print current progress
printf("\n");
for (int i = 0; i < len; ++i) {
if (i < step) {
printf("  %s ->", nodes[path[i]].name);
} else if (i == step) {
printf("  %s%s%s ->", COLOR_HIGHLIGHT, nodes[path[i]].name, COLOR_RESET);
} else {
printf("  %s ->", nodes[path[i]].name);
}
}
printf(" [end]\n");
if (step < len - 1) {
printf("    关系： %s%s %s %s\n", COLOR_STEP, nodes[path[step]].name, labels[step], COLOR_RESET);
}
if (step_by_enter) {
printf("按回车继续...\n");
getchar();
} else {
SLEEP_MS(ms_delay);
}
}
    printf("\n%s推理完成：%s → %s%s\n", COLOR_HIGHLIGHT, nodes[path[0]].name, nodes[path[len-1]].name, COLOR_RESET);
}

// Initialize extended人物库与常见关系
void init_full_graph() {
// 核心贾府（荣国府 / 宁国府 等）
    add_edge_by_name("贾母", "贾赦", "母子");
    add_edge_by_name("贾母", "贾政", "母子");
    add_edge_by_name("贾赦", "邢夫人", "夫妻");
    add_edge_by_name("贾赦", "贾琏", "父子");
    add_edge_by_name("邢夫人", "贾琏", "母子");
    add_edge_by_name("贾琏", "王熙凤", "夫妻");
    add_edge_by_name("贾政", "王夫人", "夫妻");
    add_edge_by_name("贾政", "贾珠", "父子");
    add_edge_by_name("王夫人", "贾珠", "母子");
    add_edge_by_name("贾政", "贾宝玉", "父子");
    add_edge_by_name("王夫人", "贾宝玉", "母子");
    add_edge_by_name("贾政", "贾迎春", "父女");
    add_edge_by_name("贾政", "贾探春", "父女");
    add_edge_by_name("贾政", "贾惜春", "父女");
    add_edge_by_name("贾政", "贾环", "父子");
    add_edge_by_name("贾政", "赵姨娘", "夫妻(妾)");
    add_edge_by_name("赵姨娘", "贾探春", "母女"); // 注：赵姨娘生探春在某些版本记载为庶出

// 王家 / 王熙凤 相关
    add_edge_by_name("王夫人", "王熙凤", "姨侄");
    add_edge_by_name("王熙凤", "尤氏", "姐妹(姻亲界/姐妹关系)"); // 示意性连接

// 林家
    add_edge_by_name("林如海", "林黛玉", "父女");
    add_edge_by_name("贾母", "林如海", "外祖/姻亲"); // 简化的亲缘/社交连接示意
    add_edge_by_name("贾宝玉", "林黛玉", "表兄妹"); // 表兄妹
    add_edge_by_name("林黛玉", "林如海", "母系血亲(父女)");

// 薛家
    add_edge_by_name("薛宝钗", "薛姨妈", "母女");
    add_edge_by_name("薛姨妈", "王夫人", "姐妹"); // 王夫人与薛姨妈为姐妹关系的传统说法
    add_edge_by_name("薛宝钗", "贾宝玉", "表兄妹（婚配后为夫妻）");
    add_edge_by_name("贾宝玉", "薛宝钗", "表兄妹/后成为配偶");

// 史家 / 相关亲友
    add_edge_by_name("史湘云", "贾母", "侄孙女/亲戚");
    add_edge_by_name("史湘云", "贾宝玉", "表兄妹");
    add_edge_by_name("史家祖", "史湘云", "祖孙");

// 李家
    add_edge_by_name("贾珠", "李纨", "夫妻(遗孀)"); // 李纨为贾珠遗孀，育有贾兰（贾兰可以另加）
    add_edge_by_name("李纨", "贾兰", "母子");

// 贾家更多成员/支系与他们的子女（部分名字）
    add_edge_by_name("贾敬", "贾赦", "兄弟"); // 贾敬为家中其他长辈，示意
    add_edge_by_name("贾代善", "贾母", "配偶(历史/长辈)");
    add_edge_by_name("贾敬", "尤氏", "亲属(旧名连接)");

// 主要女子侍从 / 丫鬟
    add_edge_by_name("贾宝玉", "袭人", "主仆(情感亲近)");
    add_edge_by_name("贾宝玉", "晴雯", "主仆(宠妾)");
    add_edge_by_name("贾宝玉", "平儿", "主仆(丫鬟-王熙凤侍从)");
    add_edge_by_name("王熙凤", "平儿", "主仆");
    add_edge_by_name("贾宝玉", "香菱", "主仆(表妹/教养关系)");

// 薛家扩展
    add_edge_by_name("薛蟠", "薛宝钗", "兄妹");
    add_edge_by_name("薛蟠", "薛姨妈", "母子(或姻亲)"); // 薛蟠为薛家的男丁

// 尤氏、秦可卿等（示意）
    add_edge_by_name("尤氏", "贾珍", "夫妻(贾府支系)");
    add_edge_by_name("秦可卿", "贾蓉", "夫妻"); // 复杂故事线的支系

// 进一步补充更多出现在小说中的人物（许多为亲友、仆从、堂表亲）
// 列出较多名字以达到较完整的库（名字来源于常见人物表）
    const char *more[] = {
"贾兰","贾芸","贾瑞","尤二姐","尤三姐","尤氏(姻)","贾琮","刘姥姥","刘姥姥孙",
"平儿(王熙凤侍从)","小红","小翠","二凤","秦钟","尤三姐(矛盾版)","侯方域",
"甄士隐(示意)","春燕","金钏","紫鹃","荷花庵尼姑","孙绍祖","全名(补充示意)",
"赵姨娘","贾环(庶子)","贾玮","贾珍","贾蓉","尤氏(贾珍系)","李贵","米玉",
"米氏(仆役)","吴月娘","平冈","王仁","贾天镇","贾政家臣","李大夫"
    };
    int moreN = sizeof(more)/sizeof(more[0]);
    for (int i = 0; i < moreN; ++i) add_node(more[i]);

    // add some exemplar relations among these extras (示意)
    add_edge_by_name("贾兰", "李纨", "父子");
    add_edge_by_name("贾芸", "贾赦", "亲戚");
    add_edge_by_name("刘姥姥", "王熙凤", "邻里(借住/往来)");
    add_edge_by_name("赵姨娘", "贾环", "母子");
    add_edge_by_name("贾蓉", "秦可卿", "夫妻");
    add_edge_by_name("尤二姐", "尤三姐", "姐妹");
    add_edge_by_name("尤二姐", "贾珍", "姻亲(示意)");
    add_edge_by_name("紫鹃", "林黛玉", "侍从");
    add_edge_by_name("小红", "贾宝玉", "丫鬟/陪伴");
    add_edge_by_name("小翠", "王熙凤", "丫鬟");

    // 额外：把一些亲属补成联系网络，便于 BFS 连通
    add_edge_by_name("贾宝玉", "贾兰", "堂表/亲属");
    add_edge_by_name("贾宝玉", "贾芸", "亲属");
    add_edge_by_name("林黛玉", "紫鹃", "侍从");
    add_edge_by_name("薛宝钗", "薛蟠", "兄妹");
    add_edge_by_name("贾迎春", "贾赦", "父女");
    add_edge_by_name("贾惜春", "贾政", "父女");
    add_edge_by_name("贾探春", "贾政", "父女");

// 构造的连通图
}

// 主程序交互
int main() {
printf("红楼梦 — 贾府人物关系推理系统（完整版 C 版）\n");
init_full_graph();
print_graph_summary();
printf("\n操作说明：输入起点人物名（或 'exit' 退出，'dot' 导出 dot 文件，'mode' 切换播放模式）：\n");
char a[MAX_NAME_LEN], b[MAX_NAME_LEN];
int ms_delay = DEFAULT_ANIM_MS;
int step_by_enter = 0; // 0 自动播放 1 按回车逐步
while (1) {
printf("\n> 请输入起点（或命令）：");
if (!fgets(a, sizeof(a), stdin)) break;
a[strcspn(a, "\n")] = '\0';
if (strlen(a) == 0) continue;
if (strcmp(a, "exit") == 0) { printf("退出。\n"); break; }
if (strcmp(a, "dot") == 0) { export_dot("jia_family_full.dot"); continue; }
if (strcmp(a, "mode") == 0) {
step_by_enter = !step_by_enter;
printf("播放模式切换为：%s\n", step_by_enter ? "按回车逐步" : "自动播放");
continue;
}
if (strncmp(a, "delay ", 6) == 0) {
int d = atoi(a+6);
if (d > 0) { ms_delay = d; printf("延迟设置为 %d ms\n", ms_delay); }
else printf("无效延迟值\n");
continue;
}
printf("> 请输入终点：");
if (!fgets(b, sizeof(b), stdin)) break;
b[strcspn(b, "\n")] = '\0';
if (strlen(b) == 0) continue;

int ia = find_node(a), ib = find_node(b);
if (ia == -1) { printf("未找到人物：%s（请检查拼写或使用 print 列表）。\n", a); continue; }
if (ib == -1) { printf("未找到人物：%s（请检查拼写或使用 print 列表）。\n", b); continue; }

int path[MAX_NODES];
char labels[MAX_NODES][48];
int len = bfs_shortest_path(ia, ib, path, labels);
if (len == 0) {
printf("未找到从 %s 到 %s 的路径（可能图不连通）。\n", a, b);
} else {
printf("找到路径（长度 %d）：\n", len);
for (int i = 0; i < len - 1; ++i) {
printf("  %s --[%s]--> ", nodes[path[i]].name, labels[i]);
}
printf("%s\n", nodes[path[len-1]].name);
animate_path(path, labels, len, ms_delay, step_by_enter);
}
}
return 0;
}

