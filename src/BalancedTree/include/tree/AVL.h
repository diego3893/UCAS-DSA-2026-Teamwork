#ifndef AVL_H
#define AVL_H

#include <utility>

/**
 * @brief 使用AVL树进行插入操作
 * 
 * @param key 插入的键值
 * @return true 插入成功
 * @return false 键值重复，插入失败
 */
bool AVL_insert(int key);

/**
 * @brief 使用AVL树进行删除操作
 * 
 * @param key 删除的键值
 */
void AVL_remove(int key);

/**
 * @brief 使用AVL树进行查找操作
 * 
 * @param key 查找的键值
 * @return true 查找成功
 * @return false 查找失败
 */
bool AVL_search(int key);

/**
 * @brief 将两棵 AVL 树合并
 * @param t1 第一棵树的根节点
 * @param t2 第二棵树的根节点
 * @return int 合并后的根节点
 */
int AVL_merge(int t1, int t2);

/**
 * @brief 按值x分裂AVL
 * @param root 要分裂的原树根节点
 * @param val 分裂参考值
 * @return std::pair<int, int> 左树根，右树根
 */
std::pair<int, int> AVL_split(int root, int val);

#endif // AVL_H