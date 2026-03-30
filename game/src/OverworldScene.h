#pragma once

#include <EntityManager.h>
#include <ForgeEngine.h>
#include <Scene.h>
#include <Tilemap.h>

class OverworldScene : public Forge::Scene {
   public:
    void onEnter(Forge::Engine& engine) override;
    void onExit(Forge::Engine& engine) override;
    void update(Forge::Engine& engine, float deltaTime) override;
    void render(Forge::Engine& engine) override;

   private:
    Forge::Tilemap tilemap;
    Forge::EntityID playerEntity = Forge::NULL_ENTITY;
};