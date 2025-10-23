#include <gtest/gtest.h>
#include "level_manager/LevelManager.h"

// Registration and loading 
TEST(LevelManagerTest, RegisterAndLoadLevel) {
    LevelManager manager;

    LevelData level;
    level.levelID = "test_level";
    level.name = "Test Level";
    level.gravity = 5.0f;
    level.difficulty = 3;

    manager.RegisterLevel(level);
    manager.LoadLevel("test_level");

    const LevelData& current = manager.GetCurrentLevel();

    EXPECT_EQ(current.levelID, "test_level");
    EXPECT_EQ(current.name, "Test Level");
    EXPECT_FLOAT_EQ(current.gravity, 5.0f);
    EXPECT_EQ(current.difficulty, 3);
}

// Invalid level loading
TEST(LevelManagerTest, LoadInvalidLevel) {
    LevelManager manager;

    testing::internal::CaptureStderr();
    manager.LoadLevel("nonexistent");
    std::string output = testing::internal::GetCapturedStderr();

    EXPECT_TRUE(output.find("not found") != std::string::npos);
}

// Get list of all available levels
TEST(LevelManagerTest, GetAvailableLevels) {
    LevelManager manager;

    LevelData levelA;
    levelA.levelID = "level_a";
    manager.RegisterLevel(levelA);

    LevelData levelB;
    levelB.levelID = "level_b";
    manager.RegisterLevel(levelB);

    auto levels = manager.GetAvailableLevels();

    EXPECT_EQ(levels.size(), 2);
    EXPECT_NE(std::find(levels.begin(), levels.end(), "level_a"), levels.end());
    EXPECT_NE(std::find(levels.begin(), levels.end(), "level_b"), levels.end());
}
