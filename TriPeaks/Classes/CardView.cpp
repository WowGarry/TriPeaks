#include "CardView.h"
#include "CardResConfig.h"

USING_NS_CC;

CardView* CardView::create(const CardModel* cardModel) {
    CardView* ret = new CardView();
    if (ret && ret->init(cardModel)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CardView::init(const CardModel* cardModel) {
    if (!Sprite::init()) {
        return false;
    }

    _cardModel = cardModel;
    _faceSprite = nullptr;
    _backSprite = nullptr;
    _touchListener = nullptr;

    // ✅ 添加背景（白色卡牌框）
    auto background = Sprite::create("res/card_general.png");
    if (background) {
        background->setPosition(Vec2(0, 0));
        background->setColor(Color3B::WHITE);  // 确保是白色
        this->addChild(background, -1);
        this->setContentSize(background->getContentSize());
    }

    // 创建正面图片（数字）
    std::string facePath = CardResConfig::getCardFacePath(
        cardModel->getFace(),
        cardModel->getSuit()
    );
    CCLOG("Loading card: %s", facePath.c_str());
    _faceSprite = Sprite::create(facePath);
    if (_faceSprite) {
        // 缩放数字图片以适应卡牌大小
        if (background) {
            float scaleX = background->getContentSize().width * 0.8f / _faceSprite->getContentSize().width;
            float scaleY = background->getContentSize().height * 0.8f / _faceSprite->getContentSize().height;
            float scale = MIN(scaleX, scaleY);
            _faceSprite->setScale(scale);
        }
        _faceSprite->setPosition(Vec2(0, 0));
        this->addChild(_faceSprite, 1);
    }

    // 创建背面图片
    std::string backPath = CardResConfig::getCardBackPath();
    _backSprite = Sprite::create(backPath);
    if (_backSprite) {
        _backSprite->setPosition(Vec2(0, 0));
        this->addChild(_backSprite, 1);
    }

    // 设置锚点
    this->setAnchorPoint(Vec2(0.5f, 0.5f));

    // 更新显示状态
    updateDisplay();

    // 设置触摸监听
    setupTouchListener();

    return true;
}

void CardView::setClickCallback(const std::function<void(int)>& callback) {
    _clickCallback = callback;
}

void CardView::updateDisplay() {
    if (!_cardModel) {
        return;
    }

    // 根据是否翻面显示正面或背面
    if (_faceSprite) {
        _faceSprite->setVisible(_cardModel->isFaceUp());
    }
    if (_backSprite) {
        _backSprite->setVisible(!_cardModel->isFaceUp());
    }
}

void CardView::playMoveToAnimation(const cocos2d::Vec2& targetPos, float duration, const std::function<void()>& callback) {
    auto moveTo = MoveTo::create(duration, targetPos);

    if (callback) {
        auto callFunc = CallFunc::create(callback);
        auto sequence = Sequence::create(moveTo, callFunc, nullptr);
        this->runAction(sequence);
    }
    else {
        this->runAction(moveTo);
    }
}

void CardView::setClickable(bool clickable) {
    if (_touchListener) {
        _touchListener->setEnabled(clickable);
    }
}

void CardView::setupTouchListener() {
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->setSwallowTouches(true);

    _touchListener->onTouchBegan = CC_CALLBACK_2(CardView::onTouchBegan, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
}

bool CardView::onTouchBegan(Touch* touch, Event* event) {
    if (!_cardModel || !_clickCallback) {
        return false;
    }

    Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
    Size size = this->getContentSize();

    Rect rect = Rect(
        -size.width * 0.5f,
        -size.height * 0.5f,
        size.width,
        size.height
    );

    if (rect.containsPoint(locationInNode)) {
        if (_clickCallback) {
            _clickCallback(_cardModel->getCardId());
        }
        return true;
    }
    return false;
}