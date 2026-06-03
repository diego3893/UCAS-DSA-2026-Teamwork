#ifndef TREAP_H
#define TREAP_H

#include <utility>
#include <vector>

/**
 * @brief 树堆专用的节点结构体
 * 
 */
struct TreapNode {
    int key;
    int priority; // 随机优先级 (维护堆性质)
    int size;     // 子树大小
    int left;
    int right;
};

extern std::vector<TreapNode> fhq_tree;
extern int fhq_root;

/**
 * @brief 初始化树堆
 * 
 */
void initFHQ();

/**
 * @brief 分裂
 * 
 * @param root 根节点
 * @param val 分裂用的值
 * @return std::pair<int, int> 分裂后两棵树的树根
 */
std::pair<int, int> FHQ_split(int root, int val);

/**
 * @brief 合并，x的键值整体小于y
 * 
 * @param x 第一棵树的树根
 * @param y 第二棵树的树根
 * @return int 合并后的树根
 */
int FHQ_merge(int x, int y);

/**
 * @brief 树堆插入键值
 * 
 * @param key 键值
 * @return true 插入成功
 * @return false 插入失败
 */
bool FHQ_insert(int key);

/**
 * @brief AVL转Treap
 * 
 */
void convertAVLtoFHQ();

#endif // TREAP_H