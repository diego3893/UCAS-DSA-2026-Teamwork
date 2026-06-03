#include "display.h"
#include "common.h"
#include <stack>
#include <cstdio>

using namespace std;

void showTree(){
    if(rootIndex == 0){
        printf("[当前为空树]\n");
        return;
    }
    stack<TreeNode> s;
    s.push(tree[rootIndex]);
    while(!s.empty()){
        TreeNode cur = s.top();
        s.pop();
        for(int i=0; i<cur.height; ++i){
            printf("-");
        }
        printf("%c\n", cur.key);
        if(cur.right){
            s.push(tree[cur.right]);
        }
        if(cur.left){
            s.push(tree[cur.left]);
        }
    }
    return;
}