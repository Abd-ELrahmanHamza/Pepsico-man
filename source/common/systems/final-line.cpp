#include "final-line.hpp"

#pragma once

#include "../components/player.hpp"
#include "../components/final-line.hpp"

#include <glm/glm.hpp>
#include <iostream>

namespace our {
    void FinalLineSystem::update(World *world, float deltaTime) {

        // Find the player
        glm::vec3 playerPosition;
        PlayerComponent *player;
        for (auto entity: world->getEntities()) {
            // Get the player component if it exists
            player = entity->getComponent<PlayerComponent>();
            // If the player component exists
            if (player) {
                playerPosition =
                        glm::vec3(entity->getLocalToWorldMatrix() *
                                  glm::vec4(entity->localTransform.position, 1.0));
                break;
            }
        }
        if (!player) {
            return;
        }

        // Find the final line
        FinalLineComponent *finalLineComponent;
        Entity *finalLineEntity;
        for (auto entity: world->getEntities()) {
            // Get the player component if it exists
            finalLineEntity = entity;
            finalLineComponent = finalLineEntity->getComponent<FinalLineComponent>();
            glm::vec3 &finalLinePosition = finalLineEntity->localTransform.position;
            // If the player component exists
            if (finalLineComponent) {
                if (playerPosition[0] <= finalLinePosition[0]) {
#ifdef USE_SOUND
                    soundEngine->play2D("audio/finalLine.mp3");
#endif
                    this->app->changeState("winning");
                    std::cout<<"winning"<<std::endl;
                    break;
                }
            }
        }
    }
}