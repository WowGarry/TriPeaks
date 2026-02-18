#pragma once
#ifndef __LEVEL_CONFIG_H__
#define __LEVEL_CONFIG_H__

#include "cocos2d.h"
#include "CardResConfig.h"
#include <vector>

/**
 * 单张卡牌配置数据
 */
struct CardConfigData {
    int cardFace;  // 卡牌点数 (对应CardFaceType枚举值)
    int cardSuit;  // 卡牌花色 (对应CardSuitType枚举值)
    cocos2d::Vec2 position;  // 卡牌位置

    CardConfigData() : cardFace(0), cardSuit(0), position(cocos2d::Vec2::ZERO) {}
};

/**
 * 关卡配置类
 * 职责：存储单个关卡的静态配置数据
 */
class LevelConfig {
public:
    LevelConfig();
    ~LevelConfig();

    /**
     * 获取主牌堆配置数据
     * @return 主牌堆卡牌配置列表
     */
    const std::vector<CardConfigData>& getPlayfieldCards() const { return _playfieldCards; }

    /**
     * 获取备用牌堆配置数据
     * @return 备用牌堆卡牌配置列表
     */
    const std::vector<CardConfigData>& getStackCards() const { return _stackCards; }

    /**
     * 设置主牌堆配置
     */
    void setPlayfieldCards(const std::vector<CardConfigData>& cards) { _playfieldCards = cards; }

    /**
     * 设置备用牌堆配置
     */
    void setStackCards(const std::vector<CardConfigData>& cards) { _stackCards = cards; }

private:
    std::vector<CardConfigData> _playfieldCards;  // 主牌堆配置
    std::vector<CardConfigData> _stackCards;      // 备用牌堆配置
};

#endif // __LEVEL_CONFIG_H__