#pragma once

#include "utils/Int2.h"

#include <string>
#include <vector>

struct LevelData {
    // Identification
    std::string levelID;
    std::string name;              
    std::string region;            
    std::string missionName;       
    std::string description;       

    // Gameplay
    int difficulty = 1;       
    bool isCombatZone = false;
    bool isSafeZone = false; 
    bool allowRespawn = true;

    // Physics
    float gravity = 9.8f;
    float ambientWindX = 0.0f;
    float ambientWindY = 0.0f;
    float temperature = 22.0f;
    float pressure = 1013.25f;     // hPa

    // Navigation and map
    std::string tilemapPath;       // Path to map file
    std::string minimapPath;       // Minimap icon
    std::vector<Int2> spawnPoints;
    Int2 cameraStart;

    // Audio and ambience
    std::string musicTrack;
    std::string ambientSound;
    std::string weatherType;       // "rain", "snow", "clear", "storm"

    // Scripts and events
    std::vector<std::string> triggers;
    std::vector<std::string> scripts;
    std::vector<std::string> objectives;

    // Debug / test
    bool showGrid = false;
    bool enableCheats = false;
};
