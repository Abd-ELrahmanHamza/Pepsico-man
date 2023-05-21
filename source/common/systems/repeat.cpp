#include "repeat.hpp"

#pragma once

#include "../components/player.hpp"
#include "../components/can.hpp"
#include "../components/obstacle.hpp"
#include "../components/repeat.hpp"

#include <glm/glm.hpp>

namespace our {
    void RepeatSystem::update(World *world, float deltaTime, int level) {
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

        // If the player component doesn't exist, return
        if (!player) {
            return;
        }

        // Repeat the entities
        RepeatComponent *repeatComponent;
        Entity *repeatEntity;
        for (auto entity: world->getEntities()) {
            // Get the player component if it exists
            repeatEntity = entity;
            repeatComponent = repeatEntity->getComponent<RepeatComponent>();
            glm::vec3 &repeatPosition = repeatEntity->localTransform.position;
            // If the repeat component exists
            if (repeatComponent) {
                if (playerPosition[0] <= repeatPosition[0] - 5) {
                    CanComponent *canComponent = repeatEntity->getComponent<CanComponent>();
                    ObstacleComponent *obstacleComponent = repeatEntity->getComponent<ObstacleComponent>();
                    // Prevent the repeating after the end of the level
                    if (canComponent || obstacleComponent) {
                        if ((repeatPosition + repeatComponent->translation).x < -1995) {
                            world->markForRemoval(repeatEntity);
                            continue;
                        }
                    }
                    // Repeat the entity (translate it by the translation vector)
                    repeatPosition += repeatComponent->translation;
                }
            }
        }
        // Delete the entities that are marked for removal
        world->deleteMarkedEntities();
    }
}