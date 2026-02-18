#ifndef __LEVEL_CONFIG_LOADER_H__
#define __LEVEL_CONFIG_LOADER_H__

#include "cocos2d.h"
#include "LevelConfig.h"
#include <string>

/**
 * 关卡配置加载器
 * 职责：从JSON文件加载关卡配置数据
 */
class LevelConfigLoader {
public:
    /**
     * 加载关卡配置
     * @param levelId 关卡ID
     * @return 关卡配置对象指针，加载失败返回nullptr
     */
    static LevelConfig* loadLevelConfig(int levelId);

    /**
     * 从JSON文件加载配置
     * @param jsonFilePath JSON文件路径
     * @return 关卡配置对象指针，加载失败返回nullptr
     */
    static LevelConfig* loadFromJson(const std::string& jsonFilePath);

private:
    /**
     * 解析JSON数据
     */
    static bool parseJsonData(LevelConfig* config, const char* jsonContent);
};

#endif // __LEVEL_CONFIG_LOADER_H__