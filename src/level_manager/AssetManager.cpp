#include "level_manager/AssetManager.h"
#include "graphics/Texture.h"
#include "graphics/Renderer.h"
#include <iostream>

AssetManager::AssetManager(Renderer* renderer)
    : m_renderer{renderer} {}

AssetManager::~AssetManager() {
    UnloadAll();
}

bool AssetManager::LoadTexture(const std::string& key, const std::string& path) {
    if (!m_renderer) return false;
    auto tex = std::make_unique<Texture>();
    if (!tex->LoadFromFile(path, m_renderer->GetSDLRenderer())) {
        std::cerr << "AssetManager: Failed to load texture '" << path << "'\n";
        return false;
    }
    m_textures[key] = std::move(tex);
    return true;
}

Texture* AssetManager::GetTexture(const std::string& key) const {
    auto it = m_textures.find(key);
    return it != m_textures.end() ? it->second.get() : nullptr;
}

void AssetManager::UnloadAll() {
    m_textures.clear();
}
