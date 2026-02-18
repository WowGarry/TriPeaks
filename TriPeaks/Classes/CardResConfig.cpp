#include "CardResConfig.h"

const float CardResConfig::kCardWidth = 100.0f;
const float CardResConfig::kCardHeight = 140.0f;

std::string CardResConfig::getCardFacePath(CardFaceType face, CardSuitType suit) {
    if (face == CFT_NONE || suit == CST_NONE) {
        return "";
    }

    // 根据花色确定颜色前缀
    std::string colorPrefix;
    if (suit == CST_HEARTS || suit == CST_DIAMONDS) {
        colorPrefix = "big_red_";
    }
    else {
        colorPrefix = "big_black_";
    }

    // 根据点数确定后缀
    std::string faceSuffix;
    switch (face) {
    case CFT_ACE:   faceSuffix = "A"; break;
    case CFT_TWO:   faceSuffix = "2"; break;
    case CFT_THREE: faceSuffix = "3"; break;
    case CFT_FOUR:  faceSuffix = "4"; break;
    case CFT_FIVE:  faceSuffix = "5"; break;
    case CFT_SIX:   faceSuffix = "6"; break;
    case CFT_SEVEN: faceSuffix = "7"; break;
    case CFT_EIGHT: faceSuffix = "8"; break;
    case CFT_NINE:  faceSuffix = "9"; break;
    case CFT_TEN:   faceSuffix = "10"; break;
    case CFT_JACK:  faceSuffix = "J"; break;
    case CFT_QUEEN: faceSuffix = "Q"; break;
    case CFT_KING:  faceSuffix = "K"; break;
    default: return "";
    }

    // ✅ 确保路径正确
    std::string path = "res/number/" + colorPrefix + faceSuffix + ".png";
    CCLOG("Loading card: %s", path.c_str());  // 添加日志
    return path;

}

std::string CardResConfig::getCardBackPath() {
    return "res/card_general.png";
}

std::string CardResConfig::getSuitIconPath(CardSuitType suit) {
    switch (suit) {
    case CST_CLUBS:    return "res/suits/club.png";
    case CST_DIAMONDS: return "res/suits/diamond.png";
    case CST_HEARTS:   return "res/suits/heart.png";
    case CST_SPADES:   return "res/suits/spade.png";
    default: return "";
    }
}