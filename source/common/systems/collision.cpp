//
// Created by Lenovo on 5/5/2023.
//

#include "collision.hpp"

#pragma once

#include "../ecs/world.hpp"
#include "../components/collision.hpp"
#include "../components/player.hpp"
#include "../components/can.hpp"
#include "../components/obstacle.hpp"
#include "../components/repeat.hpp"
#include "../components/energy.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>

#include "../mesh/mesh-utils.hpp"
#include "../texture/texture-utils.hpp"

#include "../application.hpp"

namespace our
{
    void CollisionSystem::update(World *world, float deltaTime,int& countPepsi)
    {
        PlayerComponent *player;
        glm::vec3 playerPosition;
        Entity *playerEntity;
        for (auto entity : world->getEntities())
        {
            // Get the player component if it exists
            playerEntity = entity;
            player = playerEntity->getComponent<PlayerComponent>();
            // If the player component exists
            if (player)
            {
                playerPosition =
                    glm::vec3(playerEntity->getLocalToWorldMatrix() *
                              glm::vec4(playerEntity->localTransform.position, 1.0));
                break;
            }
        }
        if (!player)
        {
            return;
        }

        glm::vec3 playerStart = playerEntity->getComponent<CollisionComponent>()->start + playerPosition;
        glm::vec3 playerEnd = playerEntity->getComponent<CollisionComponent>()->end + playerPosition;

        // For each entity in the world
        for (auto entity : world->getEntities())
        {
            // Get the collision component if it exists
            CollisionComponent *collision = entity->getComponent<CollisionComponent>();
            // If the collision component exists
            if (collision)
            {
                auto objectPosition = entity->localTransform.position;
                glm::vec3 objectStart = collision->start + objectPosition;
                glm::vec3 objectEnd = collision->end + objectPosition;
                bool collided = true;
                for (int i = 0; i < 3; ++i)
                {
                    if (playerStart[i] > objectEnd[i] || playerEnd[i] < objectStart[i])
                    {
                        collided = false;
                        break;
                    }
                }
                if (collided)
                {
                    std::cout << "collision" << std::endl;
                    if (entity->getComponent<ObstacleComponent>())
                    {
                        //app->changeState("game-over");
                    }
                    else if (entity->getComponent<CanComponent>())
                    {
                        for (auto energybar : world->getEntities())
                        {
                            EnergyComponent *energy = energybar->getComponent<EnergyComponent>();
                            if (energy)
                            {
                                // rescale energy bar with one unit and move position
                                if(countPepsi<100)
                                {
                                    countPepsi++;
                                    std::cout << "countPepsi: "<< countPepsi << std::endl;
                                    std::cout << "countPepsi: "<< countPepsi << std::endl;
                                    energybar->localTransform.scale.x = (double) 0.145 * (double)(countPepsi /100.0) ;
                                    energybar->localTransform.position.x = -0.142 + 0.145*(countPepsi /100.0) ;
                                    std::cout << "scale: "<< energybar->localTransform.scale.x  << std::endl;
                                    //entity->localTransform.position.x = playerEntity->localTransform.position.x - 10;

                                    RepeatComponent* repeatComponent = entity->getComponent<RepeatComponent>();
                                    glm::vec3 &repeatPosition = entity->localTransform.position;
                                    // If the player component exists
                                    if (repeatComponent) {
                                            repeatPosition += repeatComponent->translation;
                                    }
                                }
                                
                                break;

                            }
                        }
                    }

                    break;
                }
            }
        }
    }
}