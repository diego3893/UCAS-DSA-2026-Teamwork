#include "display/display.h"
#include "common/common.h"
#include "tree/treap.h"
#include <cstdio>

using namespace std;

/**
 * @brief 递归用凹入表输出AVL树
 * 
 * @param idx 根节点
 * @param space 初始缩进
 */
static void printAVLTree(int idx, int space){
    if(!idx){
        return;
    }

    const int SPACE_STEP = 6; 
    space += SPACE_STEP;

    printAVLTree(tree[idx].right, space);
    for(int i=SPACE_STEP; i<space; ++i){
        printf(" ");
    }
    printf("%d(bf:%d)\n", tree[idx].key, tree[idx].bf);
    printAVLTree(tree[idx].left, space);
    return;
}

/**
 * @brief 递归用凹入表打印Treap
 * 
 * @param idx 根节点
 * @param space 初始缩进
 */
static void printTreap(int idx, int space){
    if(!idx){
        return;
    }

    const int SPACE_STEP = 6; 
    space += SPACE_STEP;

    printTreap(fhq_tree[idx].right, space);
    
    for(int i=SPACE_STEP; i<space; ++i){
        printf(" ");
    }
    printf("%d(sz:%d)\n", fhq_tree[idx].key, fhq_tree[idx].size); 
    printTreap(fhq_tree[idx].left, space);
    return;
}



void showAVLTree(){
    if(!rootIndex){
        printf(" [当前为空树]\n");
        return;
    }
    printAVLTree(rootIndex, 0);
    return;
}

void showTreap(int root) {
    if (!root) {
        printf(" [当前为空树]\n");
        return;
    }
    printTreap(root, 0);
    return;
}