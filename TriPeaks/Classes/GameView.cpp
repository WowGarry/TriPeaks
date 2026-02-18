#include "GameView.h"
#include "CardView.h"
#include "CardModel.h"

USING_NS_CC;

GameView* GameView::create() {
    GameView* ret = new GameView();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool GameView::init() {
    if (!Layer::init()) {
        return false;
    }

    _playfieldNode = nullptr;
    _trayNode = nullptr;
    _stackNode = nullptr;
    _undoButton = nullptr;
    _stackCountLabel = nullptr;

    initUI();
    return true;
}

void GameView::initUI() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    CCLOG("GameView::initUI - visibleSize: %.0f x %.0f",
        visibleSize.width, visibleSize.height);

    // 主牌区节点（y从580开始，高度1500）
    _playfieldNode = Node::create();
    _playfieldNode->setPosition(Vec2(origin.x, origin.y + 580));
    this->addChild(_playfieldNode, 1);

    // 底部区域节点（高度580）
    Node* bottomNode = Node::create();
    bottomNode->setPosition(Vec2(origin.x, origin.y));
    this->addChild(bottomNode, 2);

    // 备用牌堆节点
    _stackNode = Node::create();
    _stackPosition = Vec2(200, 290);
    _stackNode->setPosition(_stackPosition);
    bottomNode->addChild(_stackNode);

    // 底牌堆节点
    _trayNode = Node::create();
    _trayPosition = Vec2(540, 290);
    _trayNode->setPosition(_trayPosition);
    bottomNode->addChild(_trayNode);

    // 备用牌堆背景
    auto stackBg = Sprite::create("res/card_general.png");
    if (stackBg) {
        stackBg->setOpacity(80);
        _stackNode->addChild(stackBg, -1);
    }

    // 底牌堆背景
    auto trayBg = Sprite::create("res/card_general.png");
    if (trayBg) {
        trayBg->setOpacity(80);
        _trayNode->addChild(trayBg, -1);
    }

    // 备用牌堆数量标签
    _stackCountLabel = Label::createWithSystemFont("0", "Arial", 32);
    _stackCountLabel->setPosition(Vec2(200, 100));
    _stackCountLabel->setColor(Color3B::WHITE);
    bottomNode->addChild(_stackCountLabel);
}

void GameView::addPlayfieldCardView(CardView* cardView, const Vec2& position) {
    if (cardView && _playfieldNode) {
        cardView->setPosition(position);
        _playfieldNode->addChild(cardView);
        _cardViewMap[cardView->getCardId()] = cardView;
    }
}

void GameView::addStackCardView(CardView* cardView) {
    if (cardView && _stackNode) {
        cardView->setPosition(Vec2(0, 0));
        _stackNode->addChild(cardView);
        _cardViewMap[cardView->getCardId()] = cardView;
    }
}

void GameView::setTrayCardView(CardView* cardView) {
    if (!cardView || !_trayNode) return;

    // 移除旧的底牌视图（从 map 中也清掉）
    Vector<Node*> children = _trayNode->getChildren();
    for (auto child : children) {
        CardView* cv = dynamic_cast<CardView*>(child);
        if (cv) {
            _cardViewMap.erase(cv->getCardId());
        }
    }
    _trayNode->removeAllChildren();

    // 添加新的底牌视图
    cardView->setPosition(Vec2(0, 0));
    _trayNode->addChild(cardView);
    _cardViewMap[cardView->getCardId()] = cardView;
}

void GameView::updateStackCardView(const CardModel* topCard,
    const std::function<void(int)>& clickCallback) {
    if (!_stackNode) return;

    // 安全地清除旧的顶卡视图（保留背景 z=-1 的 sprite）
    auto childrenCopy = _stackNode->getChildren();  // 先复制一份
    for (auto child : childrenCopy) {
        if (child->getLocalZOrder() >= 0) {  // 只移除 z>=0 的，保留背景（z=-1）
            CardView* cv = dynamic_cast<CardView*>(child);
            if (cv) {
                _cardViewMap.erase(cv->getCardId());
            }
            child->removeFromParent();
        }
    }

    // 如果还有牌，显示新顶卡
    if (topCard) {
        CardView* newTopView = CardView::create(topCard);
        if (newTopView) {
            newTopView->setPosition(cocos2d::Vec2(0, 0));
            newTopView->setClickCallback(clickCallback);
            _stackNode->addChild(newTopView, 1);
            _cardViewMap[newTopView->getCardId()] = newTopView;
        }
    }
}

CardView* GameView::getCardViewById(int cardId) const {
    auto it = _cardViewMap.find(cardId);
    if (it != _cardViewMap.end()) {
        return it->second;
    }
    return nullptr;
}

void GameView::removeCardView(int cardId) {
    auto it = _cardViewMap.find(cardId);
    if (it != _cardViewMap.end()) {
        it->second->removeFromParent();
        _cardViewMap.erase(it);
    }
}

void GameView::createUndoButton(const std::function<void()>& callback) {
    _undoButton = ui::Button::create();
    _undoButton->setTitleText("Undo");
    _undoButton->setTitleFontSize(32);
    _undoButton->setTitleColor(Color3B::WHITE);
    _undoButton->setPosition(Vec2(880, 290));

    _undoButton->addClickEventListener([callback](Ref* sender) {
        if (callback) callback();
        });

    this->addChild(_undoButton, 10);
}

void GameView::updateStackCount(int count) {
    if (_stackCountLabel) {
        _stackCountLabel->setString(StringUtils::format("%d", count));
    }
}