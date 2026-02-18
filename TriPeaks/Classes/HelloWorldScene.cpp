#include "HelloWorldScene.h"
#include "GameController.h"  // 在 cpp 中引入

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

bool HelloWorld::init()
{
    if (!Scene::init())
    {
        return false;
    }

    // 创建游戏控制器并启动游戏
    _gameController = new GameController();
    _gameController->startGame(1, this);  // 加载关卡1

    return true;
}