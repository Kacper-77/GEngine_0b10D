#include "level_manager/SceneManager.h"
#include "level_manager/AssetManager.h"
#include "level_manager/ResourceLoader.h"
#include <iostream>

SceneManager::SceneManager(Renderer* renderer,
                           EntityCreationSystem* ecs,
                           AssetManager* assets,
                           ResourceLoader* loader)
    : m_renderer{renderer}, m_ecs{ecs}, m_assets{assets}, m_loader{loader} {}

bool SceneManager::LoadScene(const std::string& path) {
    if (!m_loader) return false;
    bool ok = m_loader->LoadScene(path);
    if (!ok) {
        std::cerr << "SceneManager: Failed to load scene " << path << "\n";
    }
    return ok;
}

void SceneManager::UnloadScene() {
    if (m_loader) m_loader->UnloadScene();
    if (m_assets) m_assets->UnloadAll();
}
