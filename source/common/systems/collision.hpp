#pragma once

#include "../ecs/world.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include "../application.hpp"

//#define USE_SOUND
#ifdef USE_SOUND

#include <irrKlang.h>

#endif

namespace our {

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/movement.hpp"
    class CollisionSystem {
#ifdef USE_SOUND
        irrklang::ISoundEngine *soundEngine;
#endif
    public:
        CollisionSystem() {
#ifdef USE_SOUND
            soundEngine = irrklang::createIrrKlangDevice();
#endif
        }

        Application *app; // The application in which the state runs
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app) {
            this->app = app;
        }

        // This should be called every frame to update all entities containing a MovementComponent.
        void update(World *world, float deltaTime, int &countPepsi, int &heartCount, bool isSlided,
                    float &collisionStartTime);

        void decreaseHearts(World *world, int &heartCount);
    };

}
