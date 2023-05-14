#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/player.hpp"
#include "../components/free-camera-controller.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our {

    enum JumpState {
        JUMPING,
        FALLING,
        GROUNDED
    };
    enum SlideState {
        Slided,
        NORMAL,
    };

    // The free camera controller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic. 
    // For more information, see "common/components/free-camera-controller.hpp"
    class FreeCameraControllerSystem {
        Application *app; // The application in which the state runs
        bool mouse_locked = false; // Is the mouse locked

        float slideTime = 0;
        our::JumpState jumpState = our::JumpState::GROUNDED;
        our::SlideState slideState = our::SlideState::NORMAL;
    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app) {
            this->app = app;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent 
        void update(World *world, float deltaTime) {
            // First of all, we search for an cameraEntity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break
            CameraComponent *camera = nullptr;
            FreeCameraControllerComponent *controller = nullptr;
            for (auto entity: world->getEntities()) {
                camera = entity->getComponent<CameraComponent>();
                controller = entity->getComponent<FreeCameraControllerComponent>();
                if (camera && controller) break;
            }

            // Get player component
            PlayerComponent *player = nullptr;
            for (auto entity: world->getEntities()) {
                player = entity->getComponent<PlayerComponent>();
                if (player) break;
            }

            // If there is no cameraEntity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            if (!(camera && controller && player)) return;

            // Get the cameraEntity that we found via getOwner of camera (we could use controller->getOwner())
            Entity *cameraEntity = camera->getOwner();

            // Get the playerEntity that we found via getOwner of player (we could use controller->getOwner())
            Entity *playerEntity = player->getOwner();

            // We get a reference to the playerEntity's position and rotation
            glm::vec3 &position = cameraEntity->localTransform.position;
            glm::vec3 &rotation = cameraEntity->localTransform.rotation;

            // We get a reference to the playerEntity's position
            glm::vec3 &playerPosition = playerEntity->localTransform.position;
            glm::vec3 &playerRotation = playerEntity->localTransform.rotation;
            // We get a reference to the cameraEntity's position
            glm::vec3 &cameraPosition = cameraEntity->localTransform.position;

            // If the left mouse button is pressed, we lock and hide the mouse. This common in First Person Games.
            if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && !mouse_locked) {
                app->getMouse().lockMouse(app->getWindow());
                mouse_locked = true;
                // If the left mouse button is released, we unlock and unhide the mouse.
            } else if (!app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && mouse_locked) {
                app->getMouse().unlockMouse(app->getWindow());
                mouse_locked = false;
            }

            // If the left mouse button is pressed, we get the change in the mouse location
            // and use it to update the camera rotation
//            if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1)) {
//                glm::vec2 delta = app->getMouse().getMouseDelta();
//                rotation.x -= delta.y * controller->rotationSensitivity; // The y-axis controls the pitch
//                rotation.y -= delta.x * controller->rotationSensitivity; // The x-axis controls the yaw
//            }

            // We prevent the pitch from exceeding a certain angle from the XZ plane to prevent gimbal locks
            if (rotation.x < -glm::half_pi<float>() * 0.99f) rotation.x = -glm::half_pi<float>() * 0.99f;
            if (rotation.x > glm::half_pi<float>() * 0.99f) rotation.x = glm::half_pi<float>() * 0.99f;

            // This is not necessary, but whenever the rotation goes outside the 0 to 2*PI range, we wrap it back inside.
            // This could prevent floating point error if the player rotates in single direction for an extremely long time. 
            rotation.y = glm::wrapAngle(rotation.y);

            // We update the camera fov based on the mouse wheel scrolling amount
            float fov = camera->fovY + app->getMouse().getScrollOffset().y * controller->fovSensitivity;
            fov = glm::clamp(fov, glm::pi<float>() * 0.01f,
                             glm::pi<float>() * 0.99f); // We keep the fov in the range 0.01*PI to 0.99*PI
            camera->fovY = fov;

            // We get the player model matrix (relative to its parent) to compute the playerFront, playerUp and playerRight directions
            glm::mat4 playerMatrix = playerEntity->localTransform.toMat4();

            glm::vec3 playerFront = glm::vec3(playerMatrix * glm::vec4(0, 0, -1, 0)),
                    playerUp = glm::vec3(playerMatrix * glm::vec4(0, 1, 0, 0)),
                    playerRight = glm::vec3(playerMatrix * glm::vec4(1, 0, 0, 0));

            // We get the camera model matrix (relative to its parent) to compute the cameraFront, cameraUp and cameraRight directions
            glm::mat4 cameraMatrix = cameraEntity->localTransform.toMat4();

            glm::vec3 cameraFront = glm::vec3(cameraMatrix * glm::vec4(0, 0, -1, 0)),
                    cameraUp = glm::vec3(cameraMatrix * glm::vec4(0, 1, 0, 0)),
                    cameraRight = glm::vec3(cameraMatrix * glm::vec4(1, 0, 0, 0));

            glm::vec3 current_sensitivity = controller->positionSensitivity;

            // If the LEFT SHIFT key is pressed, we multiply the position sensitivity by the speed playerUp factor
            if (app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT)) current_sensitivity *= controller->speedupFactor;

            // We change the camera position based on the keys WASD/QE
            // S & W moves the player back and forth
            if (app->getKeyboard().isPressed(GLFW_KEY_W)) position += cameraFront * (deltaTime * current_sensitivity.z);
            if (app->getKeyboard().isPressed(GLFW_KEY_S)) position -= cameraFront * (deltaTime * current_sensitivity.z);
            // Q & E moves the player playerUp and down
            if (app->getKeyboard().isPressed(GLFW_KEY_Q)) position += cameraUp * (deltaTime * current_sensitivity.y);

            if (app->getKeyboard().isPressed(GLFW_KEY_E)) position += -cameraUp * (deltaTime * current_sensitivity.y);
            // A & D moves the player left or playerRight

            // Jump logic
            float jumpSpeed = 6;
            float jumpMaxHeight = 4;
            if (app->getKeyboard().isPressed(GLFW_KEY_SPACE)  || app->getKeyboard().isPressed(GLFW_KEY_UP)) {
                if (jumpState == our::JumpState::GROUNDED && slideState == our::SlideState::NORMAL) {
                    jumpState = our::JumpState::JUMPING;
                    position.y += (deltaTime * jumpSpeed);
                }
            }
            if (position.y >= jumpMaxHeight) {
                jumpState = our::JumpState::FALLING;
            } else if (position.y <= 1) {
                jumpState = our::JumpState::GROUNDED;
            }
            if (jumpState == our::JumpState::JUMPING) {
                position.y += (deltaTime * jumpSpeed);
            } else if (jumpState == our::JumpState::FALLING) {
                position.y -= (deltaTime * jumpSpeed);
            } else {
                position.y = 1;
            }

            // slide logic
            if (app->getKeyboard().isPressed(GLFW_KEY_S) || app->getKeyboard().isPressed(GLFW_KEY_DOWN) ) {
                if (slideState == our::SlideState::NORMAL && jumpState == our::JumpState::GROUNDED) {
                    slideState = our::SlideState::Slided;
                    
                    playerRotation.x -= 90;
                    //playerRotation.y += 45;
                    //playerRotation.x += 90;
                    
                    playerPosition.z -= 1;
                    playerPosition.y += 1;
                    //playerPosition.z += 2;
                    slideTime =0;
                }
            }
            if (slideState == our::SlideState::Slided) {
                slideTime += deltaTime;
                if (slideTime >= deltaTime*100) {
                    slideState = our::SlideState::NORMAL;

                    glm::vec4 actualposition = playerEntity->getLocalToWorldMatrix()* glm::vec4(playerEntity->localTransform.position,1.0);
                    std::cout<< "x ="<<actualposition.x<<" y="<<actualposition.y<<" z="<<actualposition.z<<std::endl;
                    // playerPosition.x += 2;
                    playerPosition.y -= 1;
                    playerPosition.z += 1;

                    playerRotation.x += 90;
                }
            }



            // Move player forward
            position += cameraFront * (deltaTime * 20);
            // if (jumpState == our::JumpState::GROUNDED && slideState == our::SlideState::NORMAL) {
            // Move player left and right
            if (app->getKeyboard().isPressed(GLFW_KEY_D) || app->getKeyboard().isPressed(GLFW_KEY_RIGHT) ) {
                // Stop player from going off the street
                if (cameraPosition.z > -8)
                    cameraPosition += cameraRight * (deltaTime * player->speed);
//                std::cout << "Camera Position: " << cameraPosition.x << " " << cameraPosition.z << std::endl;
            }
            if (app->getKeyboard().isPressed(GLFW_KEY_A ) || app->getKeyboard().isPressed(GLFW_KEY_LEFT)) {
                // Stop player from going off the street
                if (cameraPosition.z < 8)
                    cameraPosition -= cameraRight * (deltaTime * player->speed);
            }

            // }
        }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit() {
            if (mouse_locked) {
                mouse_locked = false;
                app->getMouse().unlockMouse(app->getWindow());
            }
        }

    };

}
