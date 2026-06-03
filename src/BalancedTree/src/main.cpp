#include "common/common.h"
#include "tree/AVL.h"
#include "display/display.h"
#include <windows.h>
#include <cstdio>

using namespace std;

int main(){
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    initTree();
    int choice, key;

    printf("========================================\n");
    printf("          平衡二叉树动态查找表        \n");
    printf("========================================\n");

    while(1){
        printf("-------------- 当前 AVL 树状态 ---------------\n");
        showTree();
        printf("\n--------------------------------------------\n");
        printf("\n请选择操作:\n");
        printf("  1. 插入结点 (Insert)\n");
        printf("  2. 删除结点 (Delete)\n");
        printf("  3. 查找结点 (Search)\n");
        printf("  0. 退出程序 (Exit)\n");
        printf(">> ");
        
        scanf("%d", &choice);

        switch(choice){
            case 1:
                printf("请输入要插入的关键字: ");
                scanf("%d", &key);
                if(AVL_search(key)){
                    printf("该键值已存在！请重新插入\n");
                    break;
                }
                AVL_insert(key);
                printf("[成功] 结点 %d 已插入。\n", key);
                break;

            case 2:
                printf("请输入要删除的关键字: ");
                scanf("%d", &key);
                if(!AVL_search(key)){
                    printf("[失败] 树中不存在关键字 %d。\n", key);
                }else{
                    AVL_remove(key);
                    printf("[成功] 结点 %d 已删除。\n", key);
                }
                break;

            case 3:
                printf("请输入要查找的关键字: ");
                scanf("%d", &key);
                if(AVL_search(key)){
                    printf("[结果] 找到了关键字 %d !\n", key);
                }else{
                    printf("[结果] 未找到了关键字 %d 。\n", key);
                }
                break;


            case 0:
                printf("感谢使用，程序退出。\n");
                system("pause");
                return 0;

            default:
                printf("无效的选项，请重新选择。\n");
                break;
        }
        system("pause");
        system("cls");
    }
    return 0;
}