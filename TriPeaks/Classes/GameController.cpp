#include "GameController.h"
#include "GameModel.h"
#include "GameView.h"
#include "CardView.h"
#include "UndoManager.h"
#include "LevelConfig.h"
#include "LevelConfigLoader.h"
#include "GameModelFromLevelGenerator.h"
#include "UndoModel.h"

USING_NS_CC;

GameController::GameController()
    : _gameModel(nullptr)
    , _gameView(nullptr)
    , _undoManager(nullptr) {
}

GameController::~GameController() {
    CC_SAFE_DELETE(_gameModel);
    CC_SAFE_DELETE(_undoManager);
}

bool GameController::startGame(int levelId, Node* parentNode) {
    CCLOG("GameController::startGame() called with levelId=%d", levelId);

    if (!parentNode) return false;

    LevelConfig* levelConfig = LevelConfigLoader::loadLevelConfig(levelId);
    if (!levelConfig) {
        CCLOG("Failed to load level config: %d", levelId);
        return false;
    }

    _gameModel = GameModelFromLevelGenerator::generateGameModel(levelConfig);
    delete levelConfig;

    if (!_gameModel) return false;

    _undoManager = new UndoManager();

    _gameView = GameView::create();
    if (!_gameView) return false;
    parentNode->addChild(_gameView);

    initView();
    return true;
}

void GameController::initView() {
    if (!_gameView || !_gameModel) return;

    // ✅ 创建主牌区卡牌视图
    const auto& playfieldCards = _gameModel->getPlayfieldCards();
    for (auto card : playfieldCards) {
        CardView* cardView = CardView::create(card);
        if (cardView) {
            int cardId = card->getCardId();
            cardView->setClickCallback([this, cardId](int id) {
                handlePlayfieldCardClick(id);
                });
            _gameView->addPlayfieldCardView(cardView, card->getPosition());
        }
    }

    // ✅ 创建备用牌堆视图（只显示顶部）
    const auto& stackCards = _gameModel->getStackCards();
    if (!stackCards.empty()) {
        CardModel* topCard = stackCards.back();
        CardView* cardView = CardView::create(topCard);
        if (cardView) {
            cardView->setClickCallback([this](int cardId) {
                handleStackClick();
                });
            _gameView->addStackCardView(cardView);
        }
    }

    // ✅ 设置底牌堆视图
    CardModel* trayCard = _gameModel->getTrayTopCard();
    if (trayCard) {
        CardView* cardView = CardView::create(trayCard);
        _gameView->setTrayCardView(cardView);
    }

    // ✅ 创建回退按钮
    _gameView->createUndoButton([this]() {
        handleUndoClick();
        });

    _gameView->updateStackCount(_gameModel->getStackCount());
}

void GameController::handlePlayfieldCardClick(int cardId) {
    CCLOG("Playfield card clicked: %d", cardId);

    if (canMoveToTray(cardId)) {
        movePlayfieldCardToTray(cardId);
    }
    else {
        CCLOG("Card cannot be moved to tray (not matching)");
    }
}

void GameController::handleStackClick() {
    CCLOG("Stack clicked");
    flipStackCardToTray();
}

void GameController::handleUndoClick() {
    CCLOG("Undo button clicked");

    if (!_undoManager->canUndo()) {
        CCLOG("No undo available");
        return;
    }

    UndoModel* undoData = _undoManager->undo();
    if (undoData) {
        performUndo(undoData);
        delete undoData;
    }
}

bool GameController::canMoveToTray(int cardId) {
    CardModel* card = _gameModel->getCardById(cardId);
    CardModel* trayCard = _gameModel->getTrayTopCard();
    if (!card || !trayCard) return false;
    return card->canMatchWith(trayCard);
}

void GameController::movePlayfieldCardToTray(int cardId) {
    CardModel* card = _gameModel->getCardById(cardId);
    CardModel* previousTrayCard = _gameModel->getTrayTopCard();
    if (!card || !previousTrayCard) return;

    // 记录回退
    UndoModel* undoData = new UndoModel(
        UAT_PLAYFIELD_TO_TRAY,
        cardId,
        card->getPosition(),
        previousTrayCard->getCardId()
    );
    _undoManager->recordAction(undoData);

    // 更新模型
    _gameModel->removePlayfieldCard(cardId);
    _gameModel->setTrayTopCard(card);

    // ✅ 获取视图并转换坐标
    CardView* cardView = _gameView->getCardViewById(cardId);
    if (!cardView) return;

    Vec2 startWorldPos = cardView->getParent()
        ->convertToWorldSpace(cardView->getPosition());
    Vec2 trayWorldPos = _gameView->convertToWorldSpace(
        _gameView->getTrayPosition()
    );

    // ✅ 移到根节点播放动画
    cardView->retain();
    cardView->removeFromParent();
    _gameView->addChild(cardView, 100);
    cardView->setPosition(startWorldPos);
    cardView->release();

    cardView->playMoveToAnimation(trayWorldPos, 0.3f, [this, cardView, card]() {
        cardView->removeFromParent();
        // 动画结束，创建底牌视图
        CardView* newTrayView = CardView::create(card);
        _gameView->setTrayCardView(newTrayView);
        });

    CCLOG("Moved card %d to tray", cardId);
}

//void GameController::flipStackCardToTray() {
//    if (_gameModel->getStackCount() == 0) {
//        CCLOG("No cards in stack");
//        return;
//    }
//
//    CardModel* previousTrayCard = _gameModel->getTrayTopCard();
//    if (!previousTrayCard) return;
//
//    // 抽取备用牌
//    CardModel* stackCard = _gameModel->popStackCard();
//    if (!stackCard) return;
//
//    // 记录回退
//    UndoModel* undoData = new UndoModel(
//        UAT_STACK_TO_TRAY,
//        stackCard->getCardId(),
//        _gameView->getStackPosition(),
//        previousTrayCard->getCardId()
//    );
//    _undoManager->recordAction(undoData);
//
//    // 更新模型
//    stackCard->setFaceUp(true);
//    _gameModel->setTrayTopCard(stackCard);
//
//    // ✅ 刷新备用牌堆显示（显示新的顶卡）
//    const auto& remainingStack = _gameModel->getStackCards();
//    const CardModel* nextTopCard = remainingStack.empty() ? nullptr : remainingStack.back();
//    _gameView->updateStackCardView(nextTopCard, [this](int cardId) {
//        handleStackClick();
//        });
//    _gameView->updateStackCount(_gameModel->getStackCount());
//
//    // ✅ 播放抽牌动画
//    CardView* animCard = CardView::create(stackCard);
//    if (animCard) {
//        _gameView->addChild(animCard, 100);
//
//        Vec2 startPos = _gameView->convertToWorldSpace(_gameView->getStackPosition());
//        animCard->setPosition(startPos);
//        Vec2 trayPos = _gameView->convertToWorldSpace(_gameView->getTrayPosition());
//
//        animCard->playMoveToAnimation(trayPos, 0.3f, [this, animCard, stackCard]() {
//            animCard->removeFromParent();
//            CardView* newTrayView = CardView::create(stackCard);
//            _gameView->setTrayCardView(newTrayView);
//            });
//    }
//
//    CCLOG("Flipped stack card to tray");
//}

void GameController::flipStackCardToTray() {
    if (_gameModel->getStackCount() == 0) {
        CCLOG("No cards in stack");
        return;
    }

    CardModel* previousTrayCard = _gameModel->getTrayTopCard();
    if (!previousTrayCard) return;

    CardModel* stackCard = _gameModel->popStackCard();
    if (!stackCard) return;

    // 记录回退
    UndoModel* undoData = new UndoModel(
        UAT_STACK_TO_TRAY,
        stackCard->getCardId(),
        _gameView->getStackPosition(),
        previousTrayCard->getCardId()
    );
    _undoManager->recordAction(undoData);

    stackCard->setFaceUp(true);
    _gameModel->setTrayTopCard(stackCard);

    // 第一步：先创建动画卡，从备用牌堆位置开始
    CardView* animCard = CardView::create(stackCard);
    if (!animCard) return;

    // 第二步：把动画卡放到低 z-order，不遮挡备用牌堆
    _gameView->addChild(animCard, 1);
    Vec2 startPos = _gameView->convertToWorldSpace(_gameView->getStackPosition());
    animCard->setPosition(startPos);
    Vec2 trayPos = _gameView->convertToWorldSpace(_gameView->getTrayPosition());

    // 第三步：动画开始后再刷新备用牌堆（animCard 已经在飞了，不会遮挡新顶卡）
    const auto& remainingStack = _gameModel->getStackCards();
    const CardModel* nextTopCard = remainingStack.empty() ? nullptr : remainingStack.back();
    _gameView->updateStackCardView(nextTopCard, [this](int cardId) {
        handleStackClick();
        });
    _gameView->updateStackCount(_gameModel->getStackCount());

    // 第四步：播放动画，结束后更新底牌堆
    animCard->playMoveToAnimation(trayPos, 0.3f, [this, animCard, stackCard]() {
        animCard->removeFromParent();
        CardView* newTrayView = CardView::create(stackCard);
        _gameView->setTrayCardView(newTrayView);
        });

    CCLOG("Flipped stack card to tray");
}

void GameController::performUndo(UndoModel* undoData) {
    if (!undoData) return;

    switch (undoData->getActionType()) {

    case UAT_PLAYFIELD_TO_TRAY: {
        int cardId = undoData->getCardId();
        CardModel* card = _gameModel->getCardById(cardId);
        CardModel* previousCard = _gameModel->getCardById(
            undoData->getPreviousTrayCardId()
        );
        if (!card || !previousCard) break;

        CardView* currentTrayView = _gameView->getCardViewById(cardId);
        if (!currentTrayView) break;

        // 获取起点世界坐标（底牌堆位置）
        Vec2 startWorldPos = currentTrayView->getParent()
            ->convertToWorldSpace(currentTrayView->getPosition());

        // 目标位置：卡牌在 playfieldNode 中的世界坐标
        // playfieldNode 偏移是 (0, 580)
        Vec2 targetLocalPos = undoData->getFromPosition();
        Vec2 targetWorldPos = Vec2(
            targetLocalPos.x,
            targetLocalPos.y + 580
        );

        // 从底牌堆移到根节点
        currentTrayView->retain();
        currentTrayView->removeFromParent();
        _gameView->addChild(currentTrayView, 100);
        currentTrayView->setPosition(startWorldPos);
        currentTrayView->release();

        // 立即恢复底牌堆
        _gameModel->setTrayTopCard(previousCard);
        CardView* prevView = CardView::create(previousCard);
        _gameView->setTrayCardView(prevView);

        // 播放反向动画
        currentTrayView->playMoveToAnimation(targetWorldPos, 0.3f,
            [this, currentTrayView, card, cardId, targetLocalPos]() {

                currentTrayView->removeFromParent();

                // 更新模型
                card->setPosition(targetLocalPos);
                card->setInTray(false);
                _gameModel->addPlayfieldCard(card);

                // 创建新的主牌区视图（可点击）
                CardView* newView = CardView::create(card);
                newView->setClickCallback([this, cardId](int id) {
                    handlePlayfieldCardClick(id);
                    });
                _gameView->addPlayfieldCardView(newView, targetLocalPos);
            }
        );
        break;
    }

    case UAT_STACK_TO_TRAY: {
        int cardId = undoData->getCardId();
        CardModel* card = _gameModel->getCardById(cardId);
        CardModel* previousCard = _gameModel->getCardById(
            undoData->getPreviousTrayCardId()
        );
        if (!card || !previousCard) break;

        CardView* currentTrayView = _gameView->getCardViewById(cardId);
        if (!currentTrayView) break;

        // 获取起点世界坐标
        Vec2 startWorldPos = currentTrayView->getParent()
            ->convertToWorldSpace(currentTrayView->getPosition());

        // 目标：备用牌堆世界坐标
        Vec2 targetWorldPos = _gameView->convertToWorldSpace(
            _gameView->getStackPosition()
        );

        // 从底牌堆移到根节点
        currentTrayView->retain();
        currentTrayView->removeFromParent();
        _gameView->addChild(currentTrayView, 100);
        currentTrayView->setPosition(startWorldPos);
        currentTrayView->release();

        // 立即恢复底牌堆
        _gameModel->setTrayTopCard(previousCard);
        CardView* prevView = CardView::create(previousCard);
        _gameView->setTrayCardView(prevView);

        // 播放反向动画
        currentTrayView->playMoveToAnimation(targetWorldPos, 0.3f,
            [this, currentTrayView, card]() {

                currentTrayView->removeFromParent();

                // 更新模型
                card->setFaceUp(false);
                card->setInTray(false);
                _gameModel->addStackCard(card);

                // 刷新备用牌堆视图
                const auto& stackCards = _gameModel->getStackCards();
                const CardModel* topCard = stackCards.empty()
                    ? nullptr : stackCards.back();
                _gameView->updateStackCardView(topCard, [this](int id) {
                    handleStackClick();
                    });
                _gameView->updateStackCount(_gameModel->getStackCount());
            }
        );
        break;
    }

    default:
        break;
    }

    CCLOG("Undo performed");
}