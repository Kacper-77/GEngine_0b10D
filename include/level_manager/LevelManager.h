#pragma once

#include "LevelData.h"

#include <unordered_map>

class LevelManager {
public:
    void RegisterLevel(const LevelData& levelData);
    void LoadLevel(const std::string& levelID);
    const LevelData& GetCurrentLevel() const;
    const std::string& GetCurrentLevelID() const;

    std::vector<std::string> GetAvailableLevels() const;

private:
    std::unordered_map<std::string, LevelData> m_levels;
    LevelData m_currentLevel;
};
