#include "common/common.h"
#include "tree/AVL.h"
#include "display/display.h"
#include <windows.h>
#include <iostream>
#include <limits>

using namespace std;

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    initTree();
    int choice, key;

    cout << "========================================" << endl;
    cout << "          平衡二叉树动态查找表        " << endl;
    cout << "========================================" << endl;

    while (true) {
        cout << "-------------- 当前 AVL 树状态 ---------------\n";
        showTree();
        cout << "\n--------------------------------------------" << endl;
        cout << "\n请选择操作:" << endl;
        cout << "  1. 插入结点 (Insert)" << endl;
        cout << "  2. 删除结点 (Delete)" << endl;
        cout << "  3. 查找结点 (Search)" << endl;
        cout << "  0. 退出程序 (Exit)" << endl;
        cout << ">> ";
        
        if (!(cin >> choice)) { // 防止输入非数字导致死循环
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "输入无效，请输入数字。" << endl;
            continue;
        }

        switch (choice) {
            case 1:
                cout << "请输入要插入的关键字: ";
                cin >> key;
                AVL_insert(key);
                cout << "[成功] 结点 " << key << " 已插入。" << endl;
                break;

            case 2:
                cout << "请输入要删除的关键字: ";
                cin >> key;
                if (!AVL_search(key)) {
                    cout << "[失败] 树中不存在关键字 " << key << "。" << endl;
                } else {
                    AVL_remove(key);
                    cout << "[成功] 结点 " << key << " 已删除。" << endl;
                }
                break;

            case 3:
                cout << "请输入要查找的关键字: ";
                cin >> key;
                if (AVL_search(key)) {
                    cout << "[结果] 找到了关键字 " << key << " !" << endl;
                } else {
                    cout << "[结果] 未找到关键字 " << key << "。" << endl;
                }
                break;


            case 0:
                cout << "感谢使用，程序退出。" << endl;
                return 0;

            default:
                cout << "无效的选项，请重新选择。" << endl;
                break;
        }
        system("pause");
        system("cls");
    }
    return 0;
}