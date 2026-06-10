#include "tree/treap.h"
#include "common/common.h"
#include <cstdlib>

using namespace std;

vector<TreapNode> fhq_tree;
int fhq_root = 0;

void initFHQ(){
    fhq_tree.clear();
    fhq_tree.push_back({0, 0, 0, 0, 0});
    fhq_root = 0;
    return;
}

/**
 * @brief 新建树堆节点
 * 
 * @param key 键值
 * @return int 节点下标
 */
static int newTreapNode(int key){
    fhq_tree.push_back({key, rand(), 1, 0, 0});
    return fhq_tree.size() - 1;
}

/**
 * @brief 更新子树大小
 * 
 * @param idx 树根
 */
static void updateTreapNode(int idx){
    if(idx){
        fhq_tree[idx].size = 1;
        if(fhq_tree[idx].left){
            fhq_tree[idx].size += fhq_tree[fhq_tree[idx].left].size;
        }
        if(fhq_tree[idx].right){
            fhq_tree[idx].size += fhq_tree[fhq_tree[idx].right].size;
        }
    }
    return;
}


pair<int, int> FHQ_split(int root, int val){
    if(!root){
        return {0, 0};
    }

    if(fhq_tree[root].key <= val){
        pair<int, int> p = FHQ_split(fhq_tree[root].right, val); // 分裂的点一定在右子树
        fhq_tree[root].right = p.first; // 将<=val的部分接在第一棵树上
        updateTreapNode(root);
        return {root, p.second};
    }else{
        pair<int, int> p = FHQ_split(fhq_tree[root].left, val); // 同理可得
        fhq_tree[root].left = p.second;
        updateTreapNode(root);
        return {p.first, root};
    }
}


int FHQ_merge(int x, int y){
    if(!x){
        return y;
    }
    if(!y){
        return x;
    }

    if(fhq_tree[x].priority > fhq_tree[y].priority){ // 维护大根堆性质
        fhq_tree[x].right = FHQ_merge(fhq_tree[x].right, y); // x作为y的父亲，并且y应该在x的右子树
        updateTreapNode(x);
        return x;
    }else{
        fhq_tree[y].left = FHQ_merge(x, fhq_tree[y].left); // 同理可得
        updateTreapNode(y);
        return y;
    }
}

bool FHQ_insert(int key){
    pair<int, int> p = FHQ_split(fhq_root, key);
    int node = newTreapNode(key);
    fhq_root = FHQ_merge(FHQ_merge(p.first, node), p.second);
    return 1;
    // AVL保证无重复键值，一定可以插入
}

static void traverseAndClone(int avl_idx){
    if(!avl_idx){
        return;
    }
    FHQ_insert(tree[avl_idx].key);
    traverseAndClone(tree[avl_idx].left);
    traverseAndClone(tree[avl_idx].right);
    return;
}

void convertAVLtoFHQ(){
    initFHQ();
    traverseAndClone(rootIndex);
    return;
}