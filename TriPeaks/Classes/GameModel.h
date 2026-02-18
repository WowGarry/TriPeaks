#pragma once
#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include "cocos2d.h"
#include "CardModel.h"
#include <vector>
#include <map>

/**
 * 游戏数据模型
 * 职责：存储整个游戏的运行时状态数据
 */
class GameModel {
public:
    GameModel();
    ~GameModel();

    /**
     * 添加卡牌到主牌区
     */
    void addPlayfieldCard(CardModel* card);

    /**
     * 添加卡牌到备用牌堆
     */
    void addStackCard(CardModel* card);

    /**
     * 设置底牌堆顶部卡牌
     */
    void setTrayTopCard(CardModel* card);

    /**
     * 获取底牌堆顶部卡牌
     */
    CardModel* getTrayTopCard() const { return _trayTopCard; }

    /**
     * 获取所有主牌区卡牌
     */
    const std::vector<CardModel*>& getPlayfieldCards() const { return _playfieldCards; }

    /**
     * 获取备用牌堆卡牌
     */
    const std::vector<CardModel*>& getStackCards() const { return _stackCards; }

    /**
     * 根据ID获取卡牌
     */
    CardModel* getCardById(int cardId) const;

    /**
     * 从主牌区移除卡牌
     */
    void removePlayfieldCard(int cardId);

    /**
     * 从备用牌堆移除卡牌
     */
    CardModel* popStackCard();

    /**
     * 获取当前备用牌堆剩余数量
     */
    int getStackCount() const { return _stackCards.size(); }

    /**
     * 清理所有数据
     */
    void clear();

private:
    std::vector<CardModel*> _playfieldCards;   // 主牌区卡牌列表
    std::vector<CardModel*> _stackCards;       // 备用牌堆列表
    CardModel* _trayTopCard;                   // 底牌堆顶部卡牌
    std::map<int, CardModel*> _cardMap;        // 卡牌ID映射表，便于快速查找
};

#endif // __GAME_MODEL_H__