#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <map>
#include <functional>

class CardView;
class CardModel;  // 关键

class GameView : public cocos2d::Layer {
public:
    static GameView* create();
    virtual bool init() override;

    void addPlayfieldCardView(CardView* cardView, const cocos2d::Vec2& position);
    void addStackCardView(CardView* cardView);
    void setTrayCardView(CardView* cardView);
    CardView* getCardViewById(int cardId) const;
    void removeCardView(int cardId);
    void createUndoButton(const std::function<void()>& callback);
    void updateStackCount(int count);

    // 注意参数类型要
    void updateStackCardView(const CardModel* topCard,
        const std::function<void(int)>& clickCallback);

    cocos2d::Vec2 getTrayPosition() const { return _trayPosition; }
    cocos2d::Vec2 getStackPosition() const { return _stackPosition; }

private:
    cocos2d::Node* _playfieldNode;
    cocos2d::Node* _trayNode;
    cocos2d::Node* _stackNode;
    cocos2d::Vec2 _trayPosition;
    cocos2d::Vec2 _stackPosition;
    std::map<int, CardView*> _cardViewMap;
    cocos2d::ui::Button* _undoButton;
    cocos2d::Label* _stackCountLabel;

    void initUI();
};

#endif // __GAME_VIEW_H__