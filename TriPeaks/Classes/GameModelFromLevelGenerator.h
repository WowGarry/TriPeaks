#pragma once
#ifndef __GAME_MODEL_FROM_LEVEL_GENERATOR_H__
#define __GAME_MODEL_FROM_LEVEL_GENERATOR_H__

#include "GameModel.h"
#include "LevelConfig.h"

/**
 * 游戏模型生成服务
 * 职责：将静态关卡配置转换为运行时游戏数据模型
 */
class GameModelFromLevelGenerator {
public:
    /**
     * 从关卡配置生成游戏模型
     * @param levelConfig 关卡配置
     * @return 游戏模型指针
     */
    static GameModel* generateGameModel(const LevelConfig* levelConfig);

private:
    static int s_nextCardId;  // 卡牌ID生成器

    /**
     * 生成唯一卡牌ID
     */
    static int generateCardId();
};

#endif // __GAME_MODEL_FROM_LEVEL_GENERATOR_H__