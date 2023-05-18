#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <systems/collision.hpp>
#include <systems/repeat.hpp>
#include <systems/final-line.hpp>
#include <asset-loader.hpp>

#include <irrKlang.h>

// This state shows how to use the ECS framework and deserialization.
class Playstate : public our::State {

    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;
    our::CollisionSystem collisionSystem;
    our::RepeatSystem repeatSystem;
    our::FinalLineSystem finalLineSystem;
    irrklang::ISoundEngine *soundEngine;
    int countPepsi = 0;
    int heartCount = 3;

    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        auto &config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if (config.contains("assets")) {
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        int level = getApp()->levelState;
        if (level == 1) {
            world.level = 1;
            if (config.contains("level1")) {
                std::cout << "level1 is rendered" << std::endl;
                world.deserialize(config["level1"]);
            } else if (config.contains("world")) {
                std::cout << "world is rendered" << std::endl;
                world.deserialize(config["world"]);
            }
        } else if (level == 2) {
            world.level = 2;
            if (config.contains("level2")) {
                std::cout << "level2 is rendered" << std::endl;
                world.deserialize(config["level2"]);
            } else if (config.contains("world")) {
                std::cout << "world is rendered" << std::endl;
                world.deserialize(config["world"]);
            }
        } else if (level == 3) {
            world.level = 3;
            if (config.contains("level3")) {
                std::cout << "level3 is rendered" << std::endl;
                world.deserialize(config["level3"]);
            } else if (config.contains("world")) {
                std::cout << "world is rendered" << std::endl;
                world.deserialize(config["world"]);
            }
        } else {
            if (config.contains("world")) {
                std::cout << "world is rendered" << std::endl;
                world.deserialize(config["world"]);
            }
        }

        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        collisionSystem.enter(getApp());
        finalLineSystem.enter(getApp());
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);

        // Plat state sound
        soundEngine = irrklang::createIrrKlangDevice();
        soundEngine->play2D("audio/playState.mp3", true);
    }

    void onDraw(double deltaTime) override {
        // Here, we just run a bunch of systems to control the world logic
        int level = getApp()->levelState;
        movementSystem.update(&world, (float) deltaTime, getApp()->motionState);
        cameraController.update(&world, (float) deltaTime, getApp()->motionState);
        collisionSystem.update(&world, (float) deltaTime, getApp()->countPepsi, getApp()->heartCount);

        repeatSystem.update(&world, (float) deltaTime, level);
        finalLineSystem.update(&world, (float) deltaTime);

        // And finally we use the renderer system to draw the scene
        renderer.render(&world);

        // Get a reference to the keyboard object
        auto &keyboard = getApp()->getKeyboard();

        if (keyboard.justPressed(GLFW_KEY_ESCAPE)) {
            // If the escape  key is pressed in this frame, go to the play state
            getApp()->changeState("menu");
        }
    }

    void onImmediateGui() override {
        if (getApp()->motionState == our::MotionState::RESTING) {
            ImGui::Begin("Start running", 0,
                         ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);
            // setting window position
            ImGui::SetWindowPos(ImVec2(270, 500));

            // setting window size
            ImGui::SetWindowSize(ImVec2(800, 100));
            ImGui::SetWindowFontScale(5.0f);

            // writing text to window
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "PRESS ENTER TO START");

            ImGui::End();
        }
    }

    void onDestroy() override {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        // Clear the world
        world.clear();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();

        // Stop play state sound
        soundEngine->drop();
        std::cout << "destroyed world" << std::endl;
    }
};