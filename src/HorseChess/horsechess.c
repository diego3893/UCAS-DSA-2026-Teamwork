#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

// ==================== 全局常量与数据结构 ====================

// 马的8个移动方向 (对应HTry1和HTry2)
const int dx[] = {-2, -1, 1, 2,  2,  1, -1, -2};
const int dy[] = { 1,  2, 2, 1, -1, -2, -2, -1};

typedef struct {
    int x, y;   // 马的当前坐标
    int dir;    // 马下一步的方向索引 (在排序后的候选列表中的位置)
} StackNode;

static StackNode stack[64]; // 棋盘最多走64步
static int top = -1;

#define pop()    (void)stack[top--]
#define isEmpty() (top == -1)

// 模拟栈的 push
static void push(StackNode node) {
    stack[++top] = node;
}

// ==================== 棋盘与统计 ====================

static int  board[8][8];          // 0=未访问, 1~64=访问顺序
static int  step;                 // 当前步数 (1~64)
static int  verbose;              // 是否开启回溯可视化
static int  maxSolutions;         // 目标找到的路径数 (0=不限制,直到搜完或手动停止)
static int  solutionsFound;       // 已找到的路径数
static int  userAbort;            // 用户请求中止
static long long nodesVisited;    // 累计前进步数 (探索节点)
static long long backtracks;      // 累计回溯次数
static clock_t tStart;            // 搜索开始时间

// ==================== 辅助函数 ====================

// 计算某格到棋盘中心的曼哈顿距离 (×2避免浮点)
static int centerDistance(int x, int y) {
    return abs(2 * x - 7) + abs(2 * y - 7);
}

// 输出完整数字方阵
static void printBoard(void) {
    printf("\n");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%4d", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// 输出可视化棋盘 (K=棋子当前位置, *=已走过, .=未走过)
static void printPathBoard(void) {
    printf("\n  +----------------+\n");
    for (int i = 0; i < 8; i++) {
        printf("  |");
        for (int j = 0; j < 8; j++) {
            if (i == stack[top].x && j == stack[top].y)
                printf(" K");          // 棋子当前位置
            else if (board[i][j] > 0)
                printf(" *");          // 已经走过的格子
            else
                printf(" .");          // 未访问的格子
        }
        printf(" |\n");
    }
    printf("  +----------------+\n");
    printf("  步数: %d/64, 栈深度: %d\n", step, top + 1);
}

// 清空输入缓冲区
static void clearBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// 检测用户是否按了 Q 键请求中止 (Windows)
static int checkAbort(void) {
    if (GetAsyncKeyState('Q') & 0x8000) {
        // 吞掉缓冲中的回车等，避免影响后续输入
        while (GetAsyncKeyState('Q') & 0x8000) { /* 等待松开 */ }
        return 1;
    }
    return 0;
}

// ==================== 核心搜索算法 ====================

/**
 * @brief 使用Warnsdorff启发式 + 栈模拟DFS 
 *
 * @param startX 起始横坐标
 * @param startY 起始纵坐标
 * @return int  1=找到至少一条路径, 0=未找到
 *
 * 算法要点:
 *  1. 每次选择候选位置时优先选出路最少(Warnsdorff)的格子
 *  2. 出路相同时优先选距离中心更远的格子(尽早处理边缘)
 *  3. 当候选的出路=0且尚未走完63步时, 剪枝跳过(死路预判)
 *  4. 找到一条路径后, 若maxSolutions>1则回溯继续搜索
 */
static int solve(int startX, int startY) {
    /* ---------- 初始化 ---------- */
    memset(board, 0, sizeof(board));
    board[startX][startY] = 1;
    top = -1;
    push((StackNode){startX, startY, -1});
    step           = 1;
    solutionsFound = 0;
    nodesVisited   = 0;
    backtracks     = 0;
    userAbort      = 0;
    tStart         = clock();
    int foundAny   = 0;

    // 确定进度汇报间隔: 目标少就每条都报, 目标大就每10条报一次
    int reportInterval = (maxSolutions == 0 || maxSolutions > 10) ? 10 : 1;

    /* ---------- 主循环: 非递归DFS ---------- */
    while (!isEmpty() && !userAbort) {
        // unlimited 模式下不限数量, 否则找到目标数就停
        if (maxSolutions > 0 && solutionsFound >= maxSolutions) break;

        /* --- 找到一条完整路径 --- */
        if (step == 64) {
            solutionsFound++;
            foundAny = 1;
            double elapsed = (double)(clock() - tStart) / CLOCKS_PER_SEC;

            if (verbose) {
                // 可视化模式: 完整输出棋盘
                printf("\n========== 解法 #%d ==========", solutionsFound);
                printf("  (节点: %lld, 回溯: %lld, 耗时: %.1fs)",
                       nodesVisited, backtracks, elapsed);
                printBoard();
            } else {
                // 非可视化: 进度条式单行输出
                if (solutionsFound % reportInterval == 0 || solutionsFound <= 3) {
                    printf("\r已找到: %d 条 | 探索节点: %lld | 回溯: %lld | 耗时: %.1f 秒",
                           solutionsFound, nodesVisited, backtracks, elapsed);
                    fflush(stdout);
                }
                // 每找到一条解, 输出完整棋盘 (用换行分隔)
                printf("\n========== 解法 #%d ==========", solutionsFound);
                printBoard();
            }

            if (maxSolutions == 1) return 1;   // 只需一条, 立即返回

            // 检查用户是否按 Q 中止
            if (checkAbort()) {
                printf("\n\n[用户中止] 已按 Q 键, 搜索中断。\n");
                userAbort = 1;
                break;
            }

            // 回溯一步, 继续搜索其他路径
            board[stack[top].x][stack[top].y] = 0;
            pop();
            step--;
            continue;
        }

        StackNode *cur = &stack[top];

        /* --- 收集当前节点的所有合法候选 --- */
        int candidates[8], degrees[8], dists[8];
        int count = 0;

        for (int h = 0; h < 8; h++) {
            int nx = cur->x + dx[h];
            int ny = cur->y + dy[h];

            // 边界检查 + 未访问检查
            if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8) continue;
            if (board[nx][ny] != 0) continue;

            // Warnsdorff度数: 从(nx,ny)还能走几步
            int deg = 0;
            for (int k = 0; k < 8; k++) {
                int nnx = nx + dx[k];
                int nny = ny + dy[k];
                if (nnx >= 0 && nnx < 8 && nny >= 0 && nny < 8
                        && board[nnx][nny] == 0) {
                    deg++;
                }
            }

            // 死路预判: deg=0 且不是最后一步 → 此路不通
            if (deg == 0 && step < 63) continue;

            candidates[count] = h;
            degrees[count]   = deg;
            dists[count]     = centerDistance(nx, ny);
            count++;
        }

        /* --- 无候选: 回溯 --- */
        if (count == 0) {
            if (verbose) {
                printf("\n[回溯] 位置 (%d,%d) 陷入死胡同, 回退一步\n",
                       cur->x, cur->y);
                printPathBoard();
                printf("按回车继续 (输入 q 退出可视化)... ");
                int c = getchar(); if (c == 'q' || c == 'Q') verbose = 0;
                if (c != '\n') clearBuffer();
            }
            board[cur->x][cur->y] = 0;
            pop();
            step--;
            backtracks++;
            continue;
        }

        /* --- 按 Warnsdorff 规则排序候选 (选择排序) ---
         *  主键: degree 越小越好 (出路的出路越少, 越应优先访问)
         *  次键: dist  越大越好 (距离中心越远=越靠近边缘, 优先处理)
         */
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (degrees[j] < degrees[i] ||
                    (degrees[j] == degrees[i] && dists[j] > dists[i])) {
                    int t;
                    t = degrees[i];    degrees[i]    = degrees[j];    degrees[j]    = t;
                    t = dists[i];      dists[i]      = dists[j];      dists[j]      = t;
                    t = candidates[i]; candidates[i] = candidates[j]; candidates[j] = t;
                }
            }
        }

        /* --- 从 cur->dir+1 开始尝试候选 (支持回溯后跳过已试分支) --- */
        int found = 0;
        for (int i = cur->dir + 1; i < count; i++) {
            int h  = candidates[i];
            int nx = cur->x + dx[h];
            int ny = cur->y + dy[h];

            cur->dir = i;                          // 记录本次尝试的候选索引
            push((StackNode){nx, ny, -1});         // 新节点入栈
            step++;
            board[nx][ny] = step;
            nodesVisited++;

            if (verbose) {
                printf("\n[前进] (%d,%d) --> (%d,%d), "
                       "候选中排第 %d/%d (deg=%d, dist=%d)\n",
                       cur->x, cur->y, nx, ny,
                       i + 1, count, degrees[i], dists[i]);
                printPathBoard();
                printf("按回车继续 (输入 q 退出可视化)... ");
                int c = getchar(); if (c == 'q' || c == 'Q') verbose = 0;
                if (c != '\n') clearBuffer();
            }

            found = 1;
            break;
        }

        /* --- 所有候选均已尝试且均失败: 回溯 --- */
        if (!found) {
            if (verbose) {
                printf("\n[回溯] 位置 (%d,%d) 的全部 %d 个候选均已失败, 回退一步\n",
                       cur->x, cur->y, count);
                printPathBoard();
                printf("按回车继续 (输入 q 退出可视化)... ");
                int c = getchar(); if (c == 'q' || c == 'Q') verbose = 0;
                if (c != '\n') clearBuffer();
            }
            board[cur->x][cur->y] = 0;
            pop();
            step--;
            backtracks++;
        }
    }

    return foundAny;
}

// ==================== 主程序入口 ====================

int main(void) {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    int startX, startY, choice;

    printf("========================================\n");
    printf("        马踏棋盘 (Knight's Tour)        \n");
    printf("      Warnsdorff 启发式 + 回溯算法      \n");
    printf("========================================\n");
    printf("\n在 8x8 的国际象棋棋盘上, 马按\"日\"字走法,\n");
    printf("每个方格只进入一次, 走遍全部 64 格。\n\n");

    /* --- 输入起始坐标 --- */
    printf("请输入马的初始坐标 (i j, 范围 0~7, 空格分隔): ");
    while (scanf("%d %d", &startX, &startY) != 2 ||
           startX < 0 || startX >= 8 || startY < 0 || startY >= 8) {
        printf("输入无效! 请重新输入两个 0~7 之间的整数: ");
        clearBuffer();
    }
    clearBuffer();

    /* --- 选择运行模式 --- */
    printf("\n请选择运行模式:\n");
    printf("  [1] 寻找一条路径\n");
    printf("  [2] 寻找多条路径 (指定数量, 输入 0 = 不限, 搜完为止)\n");
    printf("  [3] 回溯过程可视化 (逐步演示寻找一条路径)\n");
    printf("  [4] 回溯可视化 + 寻找多条路径\n");
    printf(">> ");
    scanf("%d", &choice);
    clearBuffer();

    verbose = (choice == 3 || choice == 4);

    if (choice == 1 || choice == 3) {
        maxSolutions = 1;
    } else {
        printf("请输入想要寻找的路径数量 (0 = 不限, 直到搜完或按 Q 中止): ");
        scanf("%d", &maxSolutions);
        clearBuffer();
        if (maxSolutions < 0) maxSolutions = 0;
        if (maxSolutions == 0) {
            printf("已设为\"不限\"模式 — 将搜索全部路线。\n");
            printf("(提示: 8×8 棋盘的马踏棋盘路线超过 260 亿条, 实际无法穷举。)\n");
            printf("(      搜索过程中可按 Q 键随时中止。)\n");
        } else if (maxSolutions > 10000) {
            printf("数量较大 (%d 条), 搜索可能需要较长时间。\n", maxSolutions);
            printf("过程中可按 Q 键随时中止。\n");
        }
        if (verbose && maxSolutions > 5) {
            printf("提示: 可视化模式下建议将数量设为较小值 (如 2~5)\n");
        }
    }

    /* --- 执行搜索并计时 --- */
    clock_t t0 = clock();
    int found  = solve(startX, startY);
    clock_t t1 = clock();

    /* --- 输出统计 --- */
    printf("\n========== 搜索完毕 ==========\n");
    if (userAbort) {
        printf("[用户中止] 搜索被手动中断。\n");
    }
    if (!found) {
        printf("未找到任何合法路径。\n");
    } else {
        printf("  起始位置:   (%d, %d)\n", startX, startY);
        printf("  找到路径数: %d\n", solutionsFound);
        printf("  探索节点数: %lld\n", nodesVisited);
        printf("  回溯次数:   %lld\n", backtracks);
        printf("  耗时:       %.3f 秒\n", (double)(t1 - t0) / CLOCKS_PER_SEC);
        if (maxSolutions > 0 && solutionsFound < maxSolutions && !userAbort) {
            printf("  (搜索空间已穷尽, 该起点只有 %d 条路径)\n", solutionsFound);
        }
    }

    system("pause");
    return 0;
}
