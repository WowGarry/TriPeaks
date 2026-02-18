#include "CardModel.h"

USING_NS_CC;

CardModel::CardModel()
    : _cardId(0)
    , _face(CardResConfig::CFT_NONE)
    , _suit(CardResConfig::CST_NONE)
    , _position(Vec2::ZERO)
    , _isFaceUp(false)
    , _isInTray(false) {
}

CardModel::CardModel(int cardId, CardResConfig::CardFaceType face, CardResConfig::CardSuitType suit)
    : _cardId(cardId)
    , _face(face)
    , _suit(suit)
    , _position(Vec2::ZERO)
    , _isFaceUp(false)
    , _isInTray(false) {
}

CardModel::~CardModel() {
}

bool CardModel::canMatchWith(const CardModel* targetCard) const {
    if (!targetCard) {
        return false;
    }

    int thisFaceValue = getFaceValue();
    int targetFaceValue = targetCard->getFaceValue();

    // 点数差值为1即可匹配（无花色限制）
    int diff = abs(thisFaceValue - targetFaceValue);
    return diff == 1;
}

int CardModel::getFaceValue() const {
    // A=1, 2-10=2-10, J=11, Q=12, K=13
    return static_cast<int>(_face) + 1;
}