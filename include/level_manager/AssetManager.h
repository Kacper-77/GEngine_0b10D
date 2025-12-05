#pragma once
#include <string>
#include <unordered_map>
#include <memory>

class Renderer;
class Texture;

class AssetManager {
public:
    explicit AssetManager(Renderer* renderer);
    ~AssetManager();

    bool LoadTexture(const std::string& key, const std::string& path);
    Texture* GetTexture(const std::string& key) const;
    void UnloadAll();

private:
    Renderer* m_renderer;
    std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
};
