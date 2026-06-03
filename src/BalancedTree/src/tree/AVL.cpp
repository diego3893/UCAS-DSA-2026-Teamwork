#include "tree/AVL.h"
#include "common/common.h"
#include <algorithm>

using namespace std;

/**
 * @brief 求高度信息
 * 
 * @param idx 节点下标
 * @return int 高度信息
 */
static int getHeight(int idx){
    return idx ? tree[idx].height : 0;
}

/**
 * @brief 更新高度和平衡因子
 * 
 * @param idx 节点下标
 */
static void updateNode(int idx){
    if(idx){
        int lh = getHeight(tree[idx].left);
        int rh = getHeight(tree[idx].right);
        tree[idx].height = 1+max(lh, rh);
        tree[idx].bf = rh-lh; 
    }
    return;
}

/**
 * @brief 右旋
 * 
 * @param y 要右旋的不平衡的节点
 * @return int 旋转后的子树根
 */
static int rightRotate(int y){
    int x = tree[y].left;
    tree[y].left = tree[x].right;
    tree[x].right = y;
    
    updateNode(y);
    updateNode(x);
    return x;
}

/**
 * @brief 左旋
 * 
 * @param x 要左旋的不平衡的节点
 * @return int 旋转后的子树根
 */
static int leftRotate(int x){
    int y = tree[x].right;
    tree[x].right = tree[y].left;
    tree[y].left = x;
    
    updateNode(x);
    updateNode(y);
    return y;
}

/**
 * @brief 插入节点
 * 
 * @param idx 平衡树的根
 * @param key 插入的键值
 * @return int 平衡后的根
 */
static int insertNode(int idx, int key, bool& isInserted){
    if(!idx){
        isInserted = 1;
        return newNode(key);
    }
    if(key < tree[idx].key){
        //修复：临时变量存扩容结果，避免vector扩容导致地址丢失而赋值失败
        int nextLeft = insertNode(tree[idx].left, key, isInserted);
        tree[idx].left = nextLeft;
    }else if(key > tree[idx].key){
        int nextRight = insertNode(tree[idx].right, key, isInserted);
        tree[idx].right = nextRight;
    }else{
        isInserted = 0;
        return idx; // 忽略重复插入
    }

    updateNode(idx); // 更新插入后的节点信息

    if(tree[idx].bf < -1){ // 左侧更高
        if(tree[tree[idx].left].bf <= 0){
            return rightRotate(idx); // RR
        }else{
            int newLeft = leftRotate(tree[idx].left); // LR
            tree[idx].left = newLeft;
            return rightRotate(idx);
        }
    }
    if(tree[idx].bf > 1){ // 右侧更高
        if(tree[tree[idx].right].bf >= 0){
            return leftRotate(idx); // LL
        }else{
            int newRight = rightRotate(tree[idx].right); // RL
            tree[idx].right = newRight;
            return leftRotate(idx);
        }
    }

    return idx;
}

/**
 * @brief 删除节点，保证节点存在
 * 
 * @param idx 平衡树根
 * @param key 删除的键值
 * @return int 删除后的平衡树根
 */
static int removeNode(int idx, int key){
    if(!idx){
        return 0;
    }

    if(key < tree[idx].key){
        int nextLeft = removeNode(tree[idx].left, key);
        tree[idx].left = nextLeft;
    }else if(key > tree[idx].key){
        int nextRight = removeNode(tree[idx].right, key);
        tree[idx].right = nextRight;
    }else{ // 找到节点
        if(!tree[idx].left || !tree[idx].right){ // 至多一棵子树
            int temp = tree[idx].left ? tree[idx].left : tree[idx].right;
            deleteNode(idx);
            return temp;
        }else{ // 如果两棵子树
            // 找它的中序直接后继
            int successor = tree[idx].right;
            while(tree[successor].left){
                successor = tree[successor].left;
            }
            tree[idx].key = tree[successor].key; // 将后继复制过来，再删除右子树的后继节点
            
            int nextRight = removeNode(tree[idx].right, tree[successor].key);
            tree[idx].right = nextRight;
        }
    }

    updateNode(idx);

    if(tree[idx].bf < -1){
        if (tree[tree[idx].left].bf <= 0){
            return rightRotate(idx); // RR
        }else{
            int newLeft = leftRotate(tree[idx].left); // LR
            tree[idx].left = newLeft;
            return rightRotate(idx);
        }
    }
    if(tree[idx].bf > 1){
        if(tree[tree[idx].right].bf >= 0){
            return leftRotate(idx); // LL
        }else{
            int newRight = rightRotate(tree[idx].right); // RL
            tree[idx].right = newRight;
            return leftRotate(idx);
        }
    }

    return idx;
}


bool AVL_insert(int key){
    bool isInserted = 0;
    rootIndex = insertNode(rootIndex, key, isInserted);
    return isInserted;
}

void AVL_remove(int key){
    rootIndex = removeNode(rootIndex, key);
    return;
}

bool AVL_search(int key){
    int cur = rootIndex;
    while(cur){
        if(key == tree[cur].key){
            return 1;
        }
        if(key < tree[cur].key){
            cur = tree[cur].left;
        }else{
            cur = tree[cur].right;
        }
    }
    return 0;
}