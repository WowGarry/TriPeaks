#include "UndoModel.h"

USING_NS_CC;

UndoModel::UndoModel()
    : _actionType(UAT_NONE)
    , _cardId(0)
    , _fromPosition(Vec2::ZERO)
    , _previousTrayCardId(0) {
}

UndoModel::UndoModel(UndoActionType type, int cardId, const cocos2d::Vec2& fromPos, int previousTrayCardId)
    : _actionType(type)
    , _cardId(cardId)
    , _fromPosition(fromPos)
    , _previousTrayCardId(previousTrayCardId) {
}

UndoModel::~UndoModel() {
}