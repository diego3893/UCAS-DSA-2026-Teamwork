#include <stdio.h>
#include <stdlib.h>

// 马的8个移动方向
const int dx[] = {-2, -1, 1, 2, 2, 1, -1, -2};
const int dy[] = {1, 2, 2, 1, -1, -2, -2, -1};

typedef struct{
    int x, y;// 马的当前坐标
    int dir;// 马下一步的方向索引
}StackNode;

StackNode stack[64];// 棋子最多只能走64步
int top = -1;// 栈的初始化

#define pop() (void)stack[top--]
#define isEmpty() (top == -1)

void push(StackNode node){
    stack[++top] = node;
}

int centerDistance(int x, int y){
    return abs(2 * x - 7) + abs(2 * y - 7);
}

int main(){
    int board[8][8] = {0};// 0表示未访问，1~63表示访问顺序
    int startX, startY;// 输入的初始坐标

    printf("Please enter the initial coordinates (i, j) (0~7, separated by space): ");
    while ((scanf("%d %d",&startX,&startY) != 2) || (startX < 0) || (startX >=8) || (startY < 0) || (startY >= 8)){
        printf("Invalid input format, please try again!\n");
        while (getchar() != '\n');// 清空输入缓冲区
    }

    // 初始化
    board[startX][startY] = 1;
    push((StackNode){startX, startY, -1});
    int step = 1;

    while (step < 64){
        StackNode *cur = &stack[top];
        int candidates[8];// 保留所有可行的方向索引
        int degrees[8];// 对应方向的度数
        int dists[8];// 对应方向移动后与中心的距离
        int count = 0;// 可行的方向总数

        for (int h = 0; h < 8; h++){
            int nx = cur->x + dx[h];
            int ny = cur->y + dy[h];
            if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8 && board[nx][ny] == 0){
                int deg = 0;
                for (int k = 0; k < 8; k++){
                    int nnx = nx + dx[k];
                    int nny = ny + dy[k];
                    if (nnx >= 0 && nnx < 8 && nny >= 0 && nny < 8 && board[nnx][nny] == 0){
                        deg++;
                    }
                }
                if (deg == 0 && step < 63){
                    continue;
                }

                candidates[count] = h;
                degrees[count] = deg;
                dists[count] = centerDistance(nx, ny);
                count++;
            }
        }
        
        if (count == 0){
            board[cur->x][cur->y] = 0;
            pop();
            step--;
            if (isEmpty()){
                printf("No valid path found!\n");
                return 1;
            }
            continue;
        }

        for (int i = 0; i < count - 1; i++){
            for (int j = i + 1; j < count; j++){
                if(degrees[j] < degrees[i] || (degrees[j] == degrees[i] && dists[j] > dists[i])){
                    int temp = degrees[i];
                    degrees[i] = degrees[j];
                    degrees[j] = temp;

                    temp = dists[i];
                    dists[i] = dists[j];
                    dists[j] = temp;

                    temp = candidates[i];
                    candidates[i] = candidates[j];
                    candidates[j] = temp;
                }
            }
        }

        int found = 0;
        for (int i = cur->dir + 1; i < count; i++){
            int h = candidates[i];
            int nx = cur->x + dx[h];
            int ny = cur->y + dy[h];

            cur->dir = i;
            push ((StackNode){nx, ny, -1});
            step++;
            board[nx][ny] = step;
            found = 1;
            break;
        }

        if (!found){
            board[cur->x][cur->y] = 0;
            pop();
            step--;
            if (isEmpty()){
                printf("No valid path found!\n");
                return 1;
            }
        }
    }

    printf("\nKnight's tour complete path:\n");
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            printf("%4d",board[i][j]);
        }
        printf("\n");
    }

    return 0;
}