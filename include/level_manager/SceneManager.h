#pragma once
#include <string>

class Renderer;
class EntityCreationSystem;
class AssetManager;
class ResourceLoader;

class SceneManager {
public:
    SceneManager(Renderer* renderer,
                 EntityCreationSystem* ecs,
                 AssetManager* assets,
                 ResourceLoader* loader);

    bool LoadScene(const std::string& path);
    void UnloadScene();

private:
    Renderer* m_renderer;
    EntityCreationSystem* m_ecs;
    AssetManager* m_assets;
    ResourceLoader* m_loader;
};
