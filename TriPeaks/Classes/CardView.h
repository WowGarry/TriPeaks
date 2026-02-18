#pragma once
#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "CardModel.h"
#include <functional>

/**
 * 卡牌视图
 * 职责：显示单张卡牌并处理点击事件
 */
class CardView : public cocos2d::Sprite {
public:
    /**
     * 创建卡牌视图
     * @param cardModel 卡牌数据模型（const指针，只读）
     * @return 卡牌视图指针
     */
    static CardView* create(const CardModel* cardModel);

    /**
     * 初始化
     */
    virtual bool init(const CardModel* cardModel);

    /**
     * 设置点击回调
     * @param callback 点击回调函数，参数为卡牌ID
     */
    void setClickCallback(const std::function<void(int)>& callback);

    /**
     * 更新卡牌显示（翻面等）
     */
    void updateDisplay();

    /**
     * 播放移动到目标位置的动画
     * @param targetPos 目标位置
     * @param duration 动画时长
     * @param callback 完成回调
     */
    void playMoveToAnimation(const cocos2d::Vec2& targetPos, float duration, const std::function<void()>& callback = nullptr);

    /**
     * 获取关联的卡牌ID
     */
    int getCardId() const { return _cardModel ? _cardModel->getCardId() : 0; }

    /**
     * 设置是否可点击
     */
    void setClickable(bool clickable);

private:
    const CardModel* _cardModel;                    // 卡牌数据模型（只读）
    cocos2d::Sprite* _faceSprite;                   // 正面图片
    cocos2d::Sprite* _backSprite;                   // 背面图片
    std::function<void(int)> _clickCallback;        // 点击回调
    cocos2d::EventListenerTouchOneByOne* _touchListener;  // 触摸监听器

    /**
     * 设置触摸监听
     */
    void setupTouchListener();

    /**
     * 触摸开始回调
     */
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif // __CARD_VIEW_H__