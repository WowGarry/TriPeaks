#include "LevelConfigLoader.h"
#include "json/document.h"

USING_NS_CC;

LevelConfig* LevelConfigLoader::loadLevelConfig(int levelId) {
    // 根据关卡ID构造文件路径
    std::string filePath = StringUtils::format("configs/level_%d.json", levelId);
    return loadFromJson(filePath);
}

LevelConfig* LevelConfigLoader::loadFromJson(const std::string& jsonFilePath) {
    // 读取JSON文件内容
    std::string content = FileUtils::getInstance()->getStringFromFile(jsonFilePath);
    if (content.empty()) {
        CCLOG("Failed to load level config file: %s", jsonFilePath.c_str());
        return nullptr;
    }

    // 创建配置对象
    LevelConfig* config = new LevelConfig();
    if (!parseJsonData(config, content.c_str())) {
        delete config;
        return nullptr;
    }

    return config;
}

bool LevelConfigLoader::parseJsonData(LevelConfig* config, const char* jsonContent) {
    if (!config || !jsonContent) {
        return false;
    }

    // 使用 Cocos2d-x 的 rapidjson
    rapidjson::Document doc;
    doc.Parse(jsonContent);

    if (doc.HasParseError()) {
        CCLOG("JSON parse error");
        return false;
    }

    // 解析主牌堆
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        const rapidjson::Value& playfieldArray = doc["Playfield"];
        std::vector<CardConfigData> playfieldCards;

        for (rapidjson::SizeType i = 0; i < playfieldArray.Size(); i++) {
            const rapidjson::Value& cardObj = playfieldArray[i];
            CardConfigData cardData;

            if (cardObj.HasMember("CardFace") && cardObj["CardFace"].IsInt()) {
                cardData.cardFace = cardObj["CardFace"].GetInt();
            }

            if (cardObj.HasMember("CardSuit") && cardObj["CardSuit"].IsInt()) {
                cardData.cardSuit = cardObj["CardSuit"].GetInt();
            }

            if (cardObj.HasMember("Position") && cardObj["Position"].IsObject()) {
                const rapidjson::Value& posObj = cardObj["Position"];
                float x = posObj.HasMember("x") ? (float)posObj["x"].GetDouble() : 0.0f;
                float y = posObj.HasMember("y") ? (float)posObj["y"].GetDouble() : 0.0f;
                cardData.position = Vec2(x, y);
            }

            playfieldCards.push_back(cardData);
        }

        config->setPlayfieldCards(playfieldCards);
    }

    // 解析备用牌堆
    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        const rapidjson::Value& stackArray = doc["Stack"];
        std::vector<CardConfigData> stackCards;

        for (rapidjson::SizeType i = 0; i < stackArray.Size(); i++) {
            const rapidjson::Value& cardObj = stackArray[i];
            CardConfigData cardData;

            if (cardObj.HasMember("CardFace") && cardObj["CardFace"].IsInt()) {
                cardData.cardFace = cardObj["CardFace"].GetInt();
            }

            if (cardObj.HasMember("CardSuit") && cardObj["CardSuit"].IsInt()) {
                cardData.cardSuit = cardObj["CardSuit"].GetInt();
            }

            if (cardObj.HasMember("Position") && cardObj["Position"].IsObject()) {
                const rapidjson::Value& posObj = cardObj["Position"];
                float x = posObj.HasMember("x") ? (float)posObj["x"].GetDouble() : 0.0f;
                float y = posObj.HasMember("y") ? (float)posObj["y"].GetDouble() : 0.0f;
                cardData.position = Vec2(x, y);
            }

            stackCards.push_back(cardData);
        }

        config->setStackCards(stackCards);
    }

    return true;
}