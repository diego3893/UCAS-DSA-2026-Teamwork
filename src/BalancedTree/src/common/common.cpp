#include "common/common.h"

using namespace std;

vector<TreeNode> tree;
vector<int> garbageBin;
int rootIndex = 0;

void initTree(){
    tree.clear();
    garbageBin.clear();
    tree.push_back({0, 0, 0, 0, 0}); // 0 号节点作为哨兵
    rootIndex = 0;
    return;
}

int newNode(int key){
    if(!garbageBin.empty()){ // 优先使用已申请但被删除的节点
        int reusedIdx = garbageBin.back();
        garbageBin.pop_back();
        tree[reusedIdx] = {key, 1, 0, 0, 0};
        return reusedIdx;
    }
    
    tree.push_back({key, 1, 0, 0, 0});
    return tree.size()-1;
}

void deleteNode(int idx){
    if(idx){
        garbageBin.push_back(idx);
    }
    return;
}