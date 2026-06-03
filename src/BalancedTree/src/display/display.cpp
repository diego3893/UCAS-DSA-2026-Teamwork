#include "display.h"
#include "common.h"
#include <stack>

using namespace std;

void showTree(){
    stack<TreeNode> s;
    s.push(tree[0]);
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