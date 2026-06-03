#ifndef COMMON_H
#define COMMON_H

#include <vector>

/**
 * @brief 树节点结构体，存左右孩子，键值，平衡因子和高度
 * 
 */
struct TreeNode{
    int key;
    int height; // 绝对高度
    int bf;     // 平衡因子=右子树高度-左子树高度
    int left;   // 左孩子下标
    int right;  // 右孩子下标
};

extern std::vector<TreeNode> tree;
extern std::vector<int> garbageBin; // 删除节点回收站
extern int rootIndex;

/**
 * @brief 初始化内存池，加入0号作为哨兵
 * 
 */
void initTree();

/**
 * @brief 新建节点
 * 
 * @param key 新节点的键值
 * @return int 新节点的下标
 */
int newNode(int key);

/**
 * @brief 删除节点，将下标号加入回收站
 * 
 * @param idx 删除节点的下标
 */
void deleteNode(int idx);

#endif // COMMON_H