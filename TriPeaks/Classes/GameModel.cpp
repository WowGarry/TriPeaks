#include "GameModel.h"

USING_NS_CC;

GameModel::GameModel()
    : _trayTopCard(nullptr) {
}

GameModel::~GameModel() {
    clear();
}

void GameModel::addPlayfieldCard(CardModel* card) {
    if (card) {
        _playfieldCards.push_back(card);
        _cardMap[card->getCardId()] = card;
    }
}

void GameModel::addStackCard(CardModel* card) {
    if (card) {
        _stackCards.push_back(card);
        _cardMap[card->getCardId()] = card;
    }
}

void GameModel::setTrayTopCard(CardModel* card) {
    _trayTopCard = card;
    if (card) {
        card->setInTray(true);
        _cardMap[card->getCardId()] = card;
    }
}

CardModel* GameModel::getCardById(int cardId) const {
    auto it = _cardMap.find(cardId);
    if (it != _cardMap.end()) {
        return it->second;
    }
    return nullptr;
}

void GameModel::removePlayfieldCard(int cardId) {
    for (auto it = _playfieldCards.begin(); it != _playfieldCards.end(); ++it) {
        if ((*it)->getCardId() == cardId) {
            _playfieldCards.erase(it);
            break;
        }
    }
}

CardModel* GameModel::popStackCard() {
    if (_stackCards.empty()) {
        return nullptr;
    }

    CardModel* card = _stackCards.back();
    _stackCards.pop_back();
    return card;
}

void GameModel::clear() {
    // 删除所有卡牌对象
    for (auto card : _playfieldCards) {
        delete card;
    }
    _playfieldCards.clear();

    for (auto card : _stackCards) {
        delete card;
    }
    _stackCards.clear();

    if (_trayTopCard) {
        delete _trayTopCard;
        _trayTopCard = nullptr;
    }

    _cardMap.clear();
}