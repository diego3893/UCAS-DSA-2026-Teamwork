#include "tree/AVL.h"
#include "common/common.h"
#include <algorithm>

using namespace std;

// ==========================================
// 辅助函数与旋转操作
// ==========================================

static int getHeight(int idx) {
    return idx ? tree[idx].height : 0;
}

// 核心状态更新：同时计算高度和平衡因子
static void updateNode(int idx) {
    if (idx) {
        int lh = getHeight(tree[idx].left);
        int rh = getHeight(tree[idx].right);
        tree[idx].height = 1 + max(lh, rh);
        tree[idx].bf = rh - lh; // 右边减左边
    }
}

// 右旋
static int rightRotate(int y) {
    int x = tree[y].left;
    tree[y].left = tree[x].right;
    tree[x].right = y;
    
    updateNode(y);
    updateNode(x);
    return x;
}

// 左旋
static int leftRotate(int x) {
    int y = tree[x].right;
    tree[x].right = tree[y].left;
    tree[y].left = x;
    
    updateNode(x);
    updateNode(y);
    return y;
}

// ==========================================
// 内部递归核心逻辑
// ==========================================

static int insertNode(int idx, int key) {
    if (idx == 0) return newNode(key);

    if (key < tree[idx].key) {
        tree[idx].left = insertNode(tree[idx].left, key);
    } else if (key > tree[idx].key) {
        tree[idx].right = insertNode(tree[idx].right, key);
    } else {
        return idx; // 忽略重复插入
    }

    // 重新计算状态
    updateNode(idx);

    // 平衡维护
    if (tree[idx].bf < -1) {
        if (tree[tree[idx].left].bf <= 0) {
            return rightRotate(idx);
        } else {
            tree[idx].left = leftRotate(tree[idx].left);
            return rightRotate(idx);
        }
    }
    if (tree[idx].bf > 1) {
        if (tree[tree[idx].right].bf >= 0) {
            return leftRotate(idx);
        } else {
            tree[idx].right = rightRotate(tree[idx].right);
            return leftRotate(idx);
        }
    }

    return idx;
}

static int removeNode(int idx, int key) {
    if (idx == 0) return 0; // 节点不存在

    if (key < tree[idx].key) {
        tree[idx].left = removeNode(tree[idx].left, key);
    } else if (key > tree[idx].key) {
        tree[idx].right = removeNode(tree[idx].right, key);
    } else {
        // 命中目标节点
        if (tree[idx].left == 0 || tree[idx].right == 0) {
            // 场景A：0 个或 1 个子节点
            int temp = tree[idx].left ? tree[idx].left : tree[idx].right;
            
            // 【重点】：将当前物理节点丢入回收站！
            deleteNode(idx);
            
            return temp;
        } else {
            // 场景B：2 个子节点
            int successor = tree[idx].right;
            while (tree[successor].left != 0) {
                successor = tree[successor].left;
            }
            
            // 值替换（狸猫换太子）
            tree[idx].key = tree[successor].key; 
            
            // 递归去右子树删除真正的“替身”物理节点
            // 替身节点必然符合场景A，它会在递归底层被 deleteNode 回收
            tree[idx].right = removeNode(tree[idx].right, tree[successor].key);
        }
    }

    // 重新计算状态
    updateNode(idx);

    // 平衡维护 (复用插入的逻辑)
    if (tree[idx].bf < -1) {
        if (tree[tree[idx].left].bf <= 0) {
            return rightRotate(idx);
        } else {
            tree[idx].left = leftRotate(tree[idx].left);
            return rightRotate(idx);
        }
    }
    if (tree[idx].bf > 1) {
        if (tree[tree[idx].right].bf >= 0) {
            return leftRotate(idx);
        } else {
            tree[idx].right = rightRotate(tree[idx].right);
            return leftRotate(idx);
        }
    }

    return idx;
}

// ==========================================
// 对外业务接口实现
// ==========================================

void AVL_insert(int key) {
    rootIndex = insertNode(rootIndex, key);
}

void AVL_remove(int key) {
    rootIndex = removeNode(rootIndex, key);
}

bool AVL_search(int key) {
    int curr = rootIndex;
    while (curr != 0) {
        if (key == tree[curr].key) return true;
        if (key < tree[curr].key) curr = tree[curr].left;
        else curr = tree[curr].right;
    }
    return false;
}