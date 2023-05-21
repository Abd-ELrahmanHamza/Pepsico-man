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
#include "../components/heart.hpp"
#include "../components/gem-heart.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>

#include "../mesh/mesh-utils.hpp"
#include "../texture/texture-utils.hpp"

#include "../application.hpp"


namespace our {
    void CollisionSystem::update(World *world, float deltaTime, int &countPepsi, int &heartCount, bool isSlided,
                                 float &collisionStartTime) {
        PlayerComponent *player; // The player component if it exists
        glm::vec3 playerPosition; // The player's position in the world
        Entity *playerEntity;   // The player entity if it exists

        for (auto entity: world->getEntities()) {   // search for the player entity
            // Get the player component if it exists
            playerEntity = entity;
            player = playerEntity->getComponent<PlayerComponent>();
            // If the player component exists
            if (player) {
                playerPosition =
                        glm::vec3(playerEntity->getLocalToWorldMatrix() *
                                  glm::vec4(playerEntity->localTransform.position, 1.0)); // get the player's position in the world
                break;
            }
        }
        if (!player) {
            return; // If the player doesn't exist, we can't do collision detection
        }

        glm::vec3 playerStart = playerEntity->getComponent<CollisionComponent>()->start + playerPosition;   // get the player's start position
        glm::vec3 playerEnd = playerEntity->getComponent<CollisionComponent>()->end + playerPosition;   // get the player's end position

        // For each entity in the world
        for (auto entity: world->getEntities()) {
            // Get the collision component if it exists
            CollisionComponent *collision = entity->getComponent<CollisionComponent>();
            // If the collision component exists
            if (collision) {
                // auto objectPosition =  glm::vec3(entity->getLocalToWorldMatrix() * glm::vec4(entity->localTransform.position,1.0) );
                auto objectPosition = entity->localTransform.position; // get the object's position in the world
                glm::vec3 objectStart = collision->start + objectPosition;  // get the object's start position
                glm::vec3 objectEnd = collision->end + objectPosition; // get the object's end position
                if (isSlided) {
                    playerStart.y = -1; 
                    playerEnd.y = 0.5;
                }
                bool collided = true;
                for (int i = 0; i < 3; ++i) {
                    if (playerStart[i] > objectEnd[i] || playerEnd[i] < objectStart[i]) { // if the player and object don't overlap on this axis
                        collided = false; // then they don't collide
                        break;
                    }
                }
                if (collided) {
                    if (entity->getComponent<ObstacleComponent>()) { // if the object is an obstacle
                        if (collisionStartTime == 0)
                            collisionStartTime = deltaTime; // start counting the time of collision for postprocessing effect
#ifdef USE_SOUND
                        std::cout << "sounnnnnnnnnd\n";
                        if (soundEngine->isCurrentlyPlaying("audio/collision.mp3"))
                            soundEngine->stopAllSounds();
                        soundEngine->play2D("audio/obstacle.mp3");
                        soundEngine->play2D("audio/collision.mp3");
#endif


#ifdef USE_SOUND
                        if (heartCount == 3) {
                            soundEngine->play2D("audio/firstDeath.mp3");
                        } else if (heartCount == 2) {
                            soundEngine->play2D("audio/secondDeath.mp3");
                        }
#endif
                        CollisionSystem::decreaseHearts(world, heartCount);

                        if (heartCount < 1) { // if the player has no more hearts

#ifdef USE_SOUND                        
                            soundEngine->play2D("audio/death.mp3");
#endif
                            app->changeState("game-over"); // go to the game over state
                        }
                    } else if (entity->getComponent<CanComponent>()) {
#ifdef USE_SOUND
                        soundEngine->play2D("audio/can.wav");
#endif
                        if (countPepsi < 100) { // if the player has less than 100 pepsi cans
                            countPepsi++; // increase the count of pepsi cans
                        }
                    }
                    else if(entity->getComponent<GemHeartComponent>()) // if the object is a gem heart
                    {
                        if(heartCount < 3) // if the player has less than 3 hearts which is max
                        {
                            heartCount++; // increase the count of hearts
                        }

                        entity->localTransform.scale = glm::vec3(0.0f, 0.0f, 0.0f); // make the gem heart disappear
                        entity->localTransform.position = glm::vec3(0.0f, 0.0f, 0.0f); // make the gem heart disappear
                        for (auto heartEntity: world->getEntities()) {  // search for the heart entity
                            HeartComponent *heart = heartEntity->getComponent<HeartComponent>(); // get the heart component if it exists
                            if (heart && heart->heartNumber == heartCount) { // if the heart component exists and it's the heart that we want to increase
                                heartEntity->localTransform.scale.x = 0.0009; // make the heart appear
                                heartEntity->localTransform.scale.y = 0.0009; // make the heart appear
                                heartEntity->localTransform.scale.z = 0.0009; // make the heart appear
                                break;
                            }
                        }
                    }
                    for (auto energybar: world->getEntities()) {
                        EnergyComponent *energy = energybar->getComponent<EnergyComponent>(); // get the energy component if it exists
                        if (energy) {
                            // rescale energy bar with one unit and move position
                            if (countPepsi < 101) {
                                energybar->localTransform.scale.x = (double) 0.145 * (double) (countPepsi / 100.0); // rescale the energy bar
                                energybar->localTransform.position.x = -0.142 + 0.145 * (countPepsi / 100.0); // move the energy bar
                            }
                            break;
                        }
                    }
                    RepeatComponent *repeatComponent = entity->getComponent<RepeatComponent>();
                    glm::vec3 &repeatPosition = entity->localTransform.position;
                    if (repeatComponent) { // if the object is a repeat object
                        repeatPosition += repeatComponent->translation; // move the object forward
                    }
                    break;
                }
            }
        }
    }

    // decrease the hearts
    void CollisionSystem::decreaseHearts(World *world, int &heartCount) { 

        for (auto heartEntity: world->getEntities()) {  // search for the heart entity
            HeartComponent *heart = heartEntity->getComponent<HeartComponent>();
            if (heart && heart->heartNumber == heartCount) { // if the heart component exists and it's the heart that we want to decrease
                heartEntity->localTransform.scale.x = 0.0;  // make the heart disappear
                heartEntity->localTransform.scale.y = 0.0;  // make the heart disappear
                heartEntity->localTransform.scale.z = 0.0;  // make the heart disappear
                heartCount--; // decrease the count of hearts
                break;
            }
        }
    }
}