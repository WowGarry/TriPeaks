#pragma once
#ifndef __UNDO_MODEL_H__
#define __UNDO_MODEL_H__

#include "cocos2d.h"

/**
 * 回退操作类型
 */
enum UndoActionType {
    UAT_NONE = 0,
    UAT_PLAYFIELD_TO_TRAY,  // 主牌区卡牌移动到底牌堆
    UAT_STACK_TO_TRAY        // 备用牌堆卡牌翻到底牌堆
};

/**
 * 单次回退数据模型
 * 职责：存储一次操作的回退信息
 */
class UndoModel {
public:
    UndoModel();
    UndoModel(UndoActionType type, int cardId, const cocos2d::Vec2& fromPos, int previousTrayCardId);
    ~UndoModel();

    // Getter方法
    UndoActionType getActionType() const { return _actionType; }
    int getCardId() const { return _cardId; }
    const cocos2d::Vec2& getFromPosition() const { return _fromPosition; }
    int getPreviousTrayCardId() const { return _previousTrayCardId; }

    // Setter方法
    void setActionType(UndoActionType type) { _actionType = type; }
    void setCardId(int cardId) { _cardId = cardId; }
    void setFromPosition(const cocos2d::Vec2& pos) { _fromPosition = pos; }
    void setPreviousTrayCardId(int cardId) { _previousTrayCardId = cardId; }

private:
    UndoActionType _actionType;         // 操作类型
    int _cardId;                        // 移动的卡牌ID
    cocos2d::Vec2 _fromPosition;        // 卡牌原始位置
    int _previousTrayCardId;            // 之前的底牌ID（用于恢复）
};

#endif // __UNDO_MODEL_H__