#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"

// 前置声明
class GameModel;
class GameView;
class UndoManager;
class LevelConfig;
class UndoModel;

/**
 * 游戏主控制器
 * 职责：协调游戏的整体流程，管理模型和视图的交互
 */
class GameController {
public:
    GameController();
    ~GameController();

    /**
     * 开始游戏
     * @param levelId 关卡ID
     * @param parentNode 父节点（用于添加GameView）
     * @return 是否成功启动
     */
    bool startGame(int levelId, cocos2d::Node* parentNode);

    /**
     * 处理主牌区卡牌点击
     * @param cardId 卡牌ID
     */
    void handlePlayfieldCardClick(int cardId);

    /**
     * 处理备用牌堆点击
     */
    void handleStackClick();

    /**
     * 处理回退按钮点击
     */
    void handleUndoClick();

private:
    GameModel* _gameModel;          // 游戏数据模型
    GameView* _gameView;            // 游戏视图
    UndoManager* _undoManager;      // 回退管理器

    /**
     * 初始化视图
     */
    void initView();

    /**
     * 主牌区卡牌移动到底牌堆
     * @param cardId 卡牌ID
     */
    void movePlayfieldCardToTray(int cardId);

    /**
     * 备用牌堆卡牌翻到底牌堆
     */
    void flipStackCardToTray();

    /**
     * 执行回退操作
     * @param undoData 回退数据
     */
    void performUndo(UndoModel* undoData);

    /**
     * 检查卡牌是否可以移动到底牌堆
     * @param cardId 卡牌ID
     * @return 是否可以移动
     */
    bool canMoveToTray(int cardId);
};

#endif // __GAME_CONTROLLER_H__