#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class GameController;

class HelloWorld : public cocos2d::Scene  // 按照教程分析，关键继承 Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(HelloWorld); 

private:
    GameController* _gameController;
};

#endif // __HELLOWORLD_SCENE_H__