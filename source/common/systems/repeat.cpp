//
// Created by Lenovo on 5/5/2023.
//

#include "repeat.hpp"

#pragma once

#include "../ecs/world.hpp"
#include "../components/collision.hpp"
#include "../components/player.hpp"
#include "../components/can.hpp"
#include "../components/obstacle.hpp"
#include "../components/repeat.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>

#include "../mesh/mesh-utils.hpp"
#include "../texture/texture-utils.hpp"

#include "../application.hpp"

namespace our {
    void RepeatSystem::update(World *world, float deltaTime) {
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

        RepeatComponent *repeatComponent;
        Entity *repeatEntity;
        for (auto entity: world->getEntities()) {
            // Get the player component if it exists
            repeatEntity = entity;
            repeatComponent = repeatEntity->getComponent<RepeatComponent>();
            glm::vec3 repeatPosition = repeatEntity->localTransform.position;
            glm::mat4 repeatMatrix = entity->localTransform.toMat4();
            glm::vec3 repeatRight = glm::vec3(repeatMatrix * glm::vec4(1, 0, 0, 0));
            // If the player component exists
            if (repeatComponent) {
                if (playerPosition[0] >= repeatPosition[0] - 2 && playerPosition[0] <= repeatPosition[0] + 2) {
                    repeatEntity->localTransform.position += repeatComponent->translation;
                }
            }
        }
    }
}