#include "GameModelFromLevelGenerator.h"

USING_NS_CC;

int GameModelFromLevelGenerator::s_nextCardId = 1;

GameModel* GameModelFromLevelGenerator::generateGameModel(const LevelConfig* levelConfig) {
    if (!levelConfig) {
        return nullptr;
    }

    GameModel* gameModel = new GameModel();

    // 生成主牌区卡牌
    const auto& playfieldCards = levelConfig->getPlayfieldCards();
    for (const auto& cardData : playfieldCards) {
        int cardId = generateCardId();
        auto cardFace = static_cast<CardResConfig::CardFaceType>(cardData.cardFace);
        auto cardSuit = static_cast<CardResConfig::CardSuitType>(cardData.cardSuit);

        CardModel* card = new CardModel(cardId, cardFace, cardSuit);
        card->setPosition(cardData.position);
        card->setFaceUp(true);  // 主牌区的牌默认翻开

        gameModel->addPlayfieldCard(card);
    }

    // 生成备用牌堆卡牌
    const auto& stackCards = levelConfig->getStackCards();
    for (const auto& cardData : stackCards) {
        int cardId = generateCardId();
        auto cardFace = static_cast<CardResConfig::CardFaceType>(cardData.cardFace);
        auto cardSuit = static_cast<CardResConfig::CardSuitType>(cardData.cardSuit);

        CardModel* card = new CardModel(cardId, cardFace, cardSuit);
        card->setFaceUp(false);  // 备用牌堆的牌默认覆盖

        gameModel->addStackCard(card);
    }

    // 设置初始底牌（从备用牌堆抽取第一张）
    if (!stackCards.empty()) {
        CardModel* firstCard = gameModel->popStackCard();
        if (firstCard) {
            firstCard->setFaceUp(true);
            gameModel->setTrayTopCard(firstCard);
        }
    }

    return gameModel;
}

int GameModelFromLevelGenerator::generateCardId() {
    return s_nextCardId++;
}