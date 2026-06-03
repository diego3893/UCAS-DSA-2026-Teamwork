#ifndef AVL_H
#define AVL_H

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

#endif // AVL_H