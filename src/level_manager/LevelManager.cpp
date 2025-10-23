#include "level_manager/LevelManager.h"
#include <iostream>

// Registration
void LevelManager::RegisterLevel(const LevelData& levelData) {
    if (levelData.levelID.empty()) {
        std::cerr << "LevelManager: Cannot register level with empty ID.\n";
        return;
    }
    m_levels[levelData.levelID] = levelData;
}

// Load level
void LevelManager::LoadLevel(const std::string& levelID) {
    auto it = m_levels.find(levelID);
    if (it != m_levels.end()) {
        m_currentLevel = it->second;
    } else {
        std::cerr << "LevelManager: Level '" << levelID << "' not found.\n";
    }
}

// Getters
const LevelData& LevelManager::GetCurrentLevel() const {
    return m_currentLevel;
}

const std::string& LevelManager::GetCurrentLevelID() const {
    return m_currentLevel.levelID;
}

 std::vector<std::string> LevelManager::GetAvailableLevels() const {
    std::vector<std::string> availableLevels;

    for (auto& [key, _]: m_levels) {
        availableLevels.push_back(key);
    }

    return availableLevels;
}
