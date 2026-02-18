#pragma once
#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include "cocos2d.h"
#include "UndoModel.h"
#include <vector>
#include <functional>

/**
 * 回退管理器
 * 职责：管理回退操作的记录和执行
 * 注意：作为Controller的成员变量，不使用单例模式
 */
class UndoManager {
public:
    UndoManager();
    ~UndoManager();

    /**
     * 记录一次操作
     * @param undoData 回退数据
     */
    void recordAction(UndoModel* undoData);

    /**
     * 执行回退
     * @return 回退数据，如果没有可回退的操作则返回nullptr
     */
    UndoModel* undo();

    /**
     * 是否有可回退的操作
     */
    bool canUndo() const;

    /**
     * 获取回退栈大小
     */
    int getUndoCount() const { return _undoStack.size(); }

    /**
     * 清空回退栈
     */
    void clear();

private:
    std::vector<UndoModel*> _undoStack;  // 回退栈
};

#endif // __UNDO_MANAGER_H__