#pragma once
#ifndef __CARD_RES_CONFIG_H__
#define __CARD_RES_CONFIG_H__

#include "cocos2d.h"
#include <string>

/**
 * 卡牌资源配置类
 * 职责：管理卡牌图片资源的路径映射
 */
class CardResConfig {
public:
    // 花色类型
    enum CardSuitType {
        CST_NONE = -1,
        CST_CLUBS,      // 梅花
        CST_DIAMONDS,   // 方块
        CST_HEARTS,     // 红桃
        CST_SPADES,     // 黑桃
        CST_NUM_CARD_SUIT_TYPES
    };

    // 正面类型
    enum CardFaceType {
        CFT_NONE = -1,
        CFT_ACE,
        CFT_TWO,
        CFT_THREE,
        CFT_FOUR,
        CFT_FIVE,
        CFT_SIX,
        CFT_SEVEN,
        CFT_EIGHT,
        CFT_NINE,
        CFT_TEN,
        CFT_JACK,
        CFT_QUEEN,
        CFT_KING,
        CFT_NUM_CARD_FACE_TYPES
    };

    /**
     * 获取卡牌正面图片路径
     * @param face 卡牌点数
     * @param suit 卡牌花色
     * @return 图片路径
     */
    static std::string getCardFacePath(CardFaceType face, CardSuitType suit);

    /**
     * 获取卡牌背面图片路径
     * @return 图片路径
     */
    static std::string getCardBackPath();

    /**
     * 获取花色图标路径
     * @param suit 花色类型
     * @return 图片路径
     */
    static std::string getSuitIconPath(CardSuitType suit);

private:
    // 卡牌尺寸常量
    static const float kCardWidth;
    static const float kCardHeight;
};

#endif // __CARD_RES_CONFIG_H__