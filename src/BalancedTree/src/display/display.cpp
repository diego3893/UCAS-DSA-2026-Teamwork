#include "display/display.h"
#include "common/common.h"
#include <cstdio>

using namespace std;

static void printTree(int idx, int space){
    if(!idx){
        return;
    }

    const int SPACE_STEP = 6; 
    space += SPACE_STEP;

    printTree(tree[idx].right, space);
    for(int i=SPACE_STEP; i<space; ++i){
        printf(" ");
    }
    printf("%d(bf:%d)\n", tree[idx].key, tree[idx].bf);
    printTree(tree[idx].left, space);
    return;
}

void showTree(){
    if(!rootIndex){
        printf(" [当前为空树]\n");
        return;
    }
    printTree(rootIndex, 0);
    return;
}