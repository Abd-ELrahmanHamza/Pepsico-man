#pragma once

#include <shader/shader.hpp>
#include <deserialize-utils.hpp>
#include <application.hpp>

// This state tests and shows how to use the Shader Class.
class ShaderTestState: public our::State {

    our::ShaderProgram* shader;
    GLuint vertex_array;
    
    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        auto& config = getApp()->getConfig()["scene"];
        // Then we load the shader that will be used for this scene
        shader = new our::ShaderProgram();
        shader->attach(config.value("vertex-shader", ""), GL_VERTEX_SHADER);
        shader->attach(config.value("fragment-shader", ""), GL_FRAGMENT_SHADER);
        shader->link();
        // Then we will set the output_type: 0=Position, 1=Color, 2=TexCoord, 3=Normal
        shader->use();
        // We loop over every uniform in the configuration and send to the program
        if(const auto& uniforms = config["uniforms"]; uniforms.is_object()){
            for(auto& [name, uniform] : uniforms.items()){
                std::string type = uniform.value("type", "");
                if(type == "float"){
                    float value = uniform.value("value", 0.0f);
                    shader->set(name, value);
                } else if(type == "int"){
                    int value = uniform.value("value", 0);
                    shader->set(name, value);
                } else if(type == "vec2"){
                    glm::vec2 value = uniform.value("value", glm::vec2(0,0));
                    shader->set(name, value);
                } else if(type == "vec3"){
                    glm::vec3 value = uniform.value("value", glm::vec3(0,0,0));
                    shader->set(name, value);
                } else if(type == "vec4"){
                    glm::vec4 value = uniform.value("value", glm::vec4(0,0,0,0));
                    shader->set(name, value);
                }
            }
        }

        glGenVertexArrays(1, &vertex_array);

        // We set the clear color to be black
        glClearColor(0.0, 0.0, 0.0, 1.0);
    }

    void onDraw(double deltaTime) override {
        // Clear the screen color
        glClear(GL_COLOR_BUFFER_BIT);
        // Use the shader then draw the mesh
        shader->use();
        glBindVertexArray(vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void onDestroy() override {
        delete shader;
        glDeleteVertexArrays(1, &vertex_array);
    }
};