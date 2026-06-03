#include "common/common.h"
#include "tree/AVL.h"
#include "display/display.h"
#include "tree/treap.h"
#include <windows.h>
#include <cstdio>
#include <utility>

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
        showAVLTree();
        printf("\n--------------------------------------------\n");
        printf("\n请选择操作:\n");
        printf("  1. 插入结点 (Insert)\n");
        printf("  2. 删除结点 (Delete)\n");
        printf("  3. 查找结点 (Search)\n");
        printf("  4. [选作] 分裂、合并平衡二叉树 (基于 FHQ Treap)\n");
        printf("  5. [选作] 分裂、合并平衡二叉树 (基于 AVL)\n");
        printf("  0. 退出程序 (Exit)\n");
        printf(">> ");
        
        scanf("%d", &choice);

        switch(choice){
            case 1:{
                printf("请输入要插入的关键字: ");
                scanf("%d", &key);
                if(AVL_insert(key)){
                    printf("[成功] 结点 %d 已插入\n", key);
                }else{
                    printf("键值重复，插入失败\n");
                }
                break;
            }

            case 2:{
                printf("请输入要删除的关键字: ");
                scanf("%d", &key);
                if(!AVL_search(key)){
                    printf("[失败] 树中不存在关键字 %d\n", key);
                }else{
                    AVL_remove(key);
                    printf("[成功] 结点 %d 已删除\n", key);
                }
                break;
            }

            case 3:{
                printf("请输入要查找的关键字: ");
                scanf("%d", &key);
                if(AVL_search(key)){
                    printf("[结果] 找到了关键字 %d\n", key);
                }else{
                    printf("[结果] 未找到了关键字 %d\n", key);
                }
                break;
            }
            case 4: {
                printf("请输入分裂参考值 x (<=x 和 >x): ");
                int split_val;
                scanf("%d", &split_val);

                convertAVLtoFHQ();
                pair<int, int> sub_trees = FHQ_split(fhq_root, split_val);
                
                printf("\n=== 分裂出的第一棵树 (<= %d) ===\n", split_val);
                showTreap(sub_trees.first);
                printf("\n=== 分裂出的第二棵树 (> %d) ===\n", split_val);
                showTreap(sub_trees.second);

                printf("\n=== 两棵树合并后如下 ===\n");
                fhq_root = FHQ_merge(sub_trees.first, sub_trees.second);
                showTreap(fhq_root);
                
                printf("\n(注：此操作在独立沙盒中演示，不影响原 AVL 树结构)\n");
                break;
            }
            case 5: {
                printf("请输入分裂参考值 x (<=x 和 >x): ");
                int split_val;
                scanf("%d", &split_val);

                pair<int, int> sub_trees = AVL_split(rootIndex, split_val);
                
                rootIndex = 0; 
                
                printf("\n=== 分裂出的第一棵树 (<= %d) ===\n", split_val);
                showAVLTree(sub_trees.first); 
                printf("\n=== 分裂出的第二棵树 (> %d) ===\n", split_val);
                showAVLTree(sub_trees.second);
                printf("\n=== 两棵树合并后如下 ===\n");
                rootIndex = AVL_merge(sub_trees.first, sub_trees.second);
                showAVLTree();
                
                break;
            }
            case 0:{
                printf("程序退出\n");
                system("pause");
                return 0;
            }
            default:{
                printf("无效的选项，请重新选择\n");
                break;
            }
        }
        system("pause");
        system("cls");
    }
    return 0;
}