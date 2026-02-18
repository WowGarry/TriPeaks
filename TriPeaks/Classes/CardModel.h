#pragma once
#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include "cocos2d.h"
#include "CardResConfig.h"

/**
 * 卡牌数据模型
 * 职责：存储单张卡牌的运行时数据
 */
class CardModel {
public:
    CardModel();
    CardModel(int cardId, CardResConfig::CardFaceType face, CardResConfig::CardSuitType suit);
    ~CardModel();

    // Getter方法
    int getCardId() const { return _cardId; }
    CardResConfig::CardFaceType getFace() const { return _face; }
    CardResConfig::CardSuitType getSuit() const { return _suit; }
    const cocos2d::Vec2& getPosition() const { return _position; }
    bool isFaceUp() const { return _isFaceUp; }
    bool isInTray() const { return _isInTray; }

    // Setter方法
    void setCardId(int cardId) { _cardId = cardId; }
    void setFace(CardResConfig::CardFaceType face) { _face = face; }
    void setSuit(CardResConfig::CardSuitType suit) { _suit = suit; }
    void setPosition(const cocos2d::Vec2& pos) { _position = pos; }
    void setFaceUp(bool faceUp) { _isFaceUp = faceUp; }
    void setInTray(bool inTray) { _isInTray = inTray; }

    /**
     * 判断是否可以与目标卡牌匹配
     * @param targetCard 目标卡牌
     * @return 是否可以匹配
     */
    bool canMatchWith(const CardModel* targetCard) const;

    /**
     * 获取卡牌点数值（1-13）
     */
    int getFaceValue() const;

private:
    int _cardId;                              // 卡牌唯一ID
    CardResConfig::CardFaceType _face;        // 卡牌点数
    CardResConfig::CardSuitType _suit;        // 卡牌花色
    cocos2d::Vec2 _position;                  // 卡牌位置
    bool _isFaceUp;                           // 是否正面朝上
    bool _isInTray;                           // 是否在底牌堆中
};

#endif // __CARD_MODEL_H__