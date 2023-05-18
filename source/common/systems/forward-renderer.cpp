#include "forward-renderer.hpp"
#include "../mesh/mesh-utils.hpp"
#include "../texture/texture-utils.hpp"
#include <iostream>
namespace our
{

    void ForwardRenderer::initialize(glm::ivec2 windowSize, const nlohmann::json &config)
    {
        // First, we store the window size for later use
        this->windowSize = windowSize;

        // Then we check if there is a sky texture in the configuration
        if (config.contains("sky"))
        {
            // First, we create a sphere which will be used to draw the sky
            this->skySphere = mesh_utils::sphere(glm::ivec2(16, 16));

            // We can draw the sky using the same shader used to draw textured objects
            ShaderProgram *skyShader = new ShaderProgram();
            skyShader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
            skyShader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
            skyShader->link();

            // TODO: (Req 10) Pick the correct pipeline state to draw the sky
            //  Hints: the sky will be draw after the opaque objects so we would need depth testing but which depth funtion should we pick?
            //  We will draw the sphere from the inside, so what options should we pick for the face culling.
            PipelineState skyPipelineState{};
            // Enable depth testing and set the depth function to LEQUAL
            // Enable depth testing to make sure the sky is drawn behind all the opaque objects
            skyPipelineState.depthTesting.enabled = true;
            // Set the depth function to LEQUAL to make sure the sky is drawn behind all the opaque objects
            skyPipelineState.depthTesting.function = GL_LEQUAL;

            // Load the sky texture (note that we don't need mipmaps since we want to avoid any unnecessary blurring while rendering the sky)
            std::string skyTextureFile = config.value<std::string>("sky", "");
            Texture2D *skyTexture = texture_utils::loadImage(skyTextureFile, false);

            // Setup a sampler for the sky
            Sampler *skySampler = new Sampler();
            skySampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_WRAP_S, GL_REPEAT);
            skySampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Combine all the aforementioned objects (except the mesh) into a material
            this->skyMaterial = new TexturedMaterial();
            this->skyMaterial->shader = skyShader;
            this->skyMaterial->texture = skyTexture;
            this->skyMaterial->sampler = skySampler;
            this->skyMaterial->pipelineState = skyPipelineState;
            this->skyMaterial->tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            this->skyMaterial->alphaThreshold = 1.0f;
            this->skyMaterial->transparent = false;
        }

        // Then we check if there is a postprocessing shader in the configuration
        if (config.contains("postprocess"))
        {
            // TODO: (Req 11) Create a framebuffer
            // we need to generate the frame buffer using our postprocess frame buffer.
            glGenFramebuffers(1, &postprocessFrameBuffer);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postprocessFrameBuffer);

            // TODO: (Req 11) Create a color and a depth texture and attach them to the framebuffer
            //  Hints: The color format can be (Red, Green, Blue and Alpha components with 8 bits for each channel).
            //  The depth format can be (Depth component with 24 bits).
            /// then we should create two empty textures to draw on them.
            colorTarget = texture_utils::empty(GL_RGBA8, windowSize);
            depthTarget = texture_utils::empty(GL_DEPTH_COMPONENT24, windowSize);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTarget->getOpenGLName(),
                                   0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTarget->getOpenGLName(), 0);
            // TODO: (Req 11) Unbind the framebuffer just to be safe
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            // Create a vertex array to use for drawing the texture
            glGenVertexArrays(1, &postProcessVertexArray);

            // Create a sampler to use for sampling the scene texture in the post processing shader
            Sampler *postprocessSampler = new Sampler();
            postprocessSampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            postprocessSampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Create the post processing shader
            ShaderProgram *postprocessShader = new ShaderProgram();
            postprocessShader->attach("assets/shaders/fullscreen.vert", GL_VERTEX_SHADER);
            postprocessShader->attach(config.value<std::string>("postprocess", ""), GL_FRAGMENT_SHADER);
            postprocessShader->link();

            // Create a post processing material
            postprocessMaterial = new TexturedMaterial();
            postprocessMaterial->shader = postprocessShader;
            postprocessMaterial->texture = colorTarget;
            postprocessMaterial->sampler = postprocessSampler;
            // The default options are fine but we don't need to interact with the depth buffer
            // so it is more performant to disable the depth mask
            postprocessMaterial->pipelineState.depthMask = false;
        }
    }

    void ForwardRenderer::destroy()
    {
        // Delete all objects related to the sky
        if (skyMaterial)
        {
            delete skySphere;
            delete skyMaterial->shader;
            delete skyMaterial->texture;
            delete skyMaterial->sampler;
            delete skyMaterial;
        }
        // Delete all objects related to post processing
        if (postprocessMaterial)
        {
            glDeleteFramebuffers(1, &postprocessFrameBuffer);
            glDeleteVertexArrays(1, &postProcessVertexArray);
            delete colorTarget;
            delete depthTarget;
            delete postprocessMaterial->sampler;
            delete postprocessMaterial->shader;
            delete postprocessMaterial;
        }
    }

    void ForwardRenderer::render(World *world)
    {
        // First of all, we search for a camera and for all the mesh renderers
        CameraComponent *camera = nullptr;
        opaqueCommands.clear();
        transparentCommands.clear();
        lights_list.clear();
        // std::unordered_set<our::Light
        for (auto entity : world->getEntities())
        {
            // If we hadn't found a camera yet, we look for a camera in this entity
            if (!camera)
                camera = entity->getComponent<CameraComponent>();
            // If this entity has a mesh renderer component
            if (auto meshRenderer = entity->getComponent<MeshRendererComponent>(); meshRenderer)
            {
                // We construct a command from it
                RenderCommand command;
                command.localToWorld = meshRenderer->getOwner()->getLocalToWorldMatrix();
                command.center = glm::vec3(command.localToWorld * glm::vec4(0, 0, 0, 1));
                command.mesh = meshRenderer->mesh;
                command.material = meshRenderer->material;
                // if it is transparent, we add it to the transparent commands list
                // transparentCommands.push_back(command);
                if (command.material->transparent)
                {
                    transparentCommands.push_back(command);
                }
                else
                {
                    // Otherwise, we add it to the opaque command list
                    opaqueCommands.push_back(command);
                }
            }
            if (auto light = entity->getComponent<LightComponent>(); light)
            {
                lights_list.push_back(light);
            }
        }

        // If there is no camera, we return (we cannot render without a camera)
        if (camera == nullptr)
            return;

        // TODO: (Req 9) Modify the following line such that "cameraForward" contains a vector pointing the camera forward direction
        //  HINT: See how you wrote the CameraComponent::getViewMatrix, it should help you solve this one
        // we need to get the entity owner of the camera,then from the entity we get the transformation matrix from it
        // using the transformation matrix we could transform the forward vector of the camera to the new coordinate
        // wich is point to the negative z direction
        auto owner = camera->getOwner();
        auto M = owner->getLocalToWorldMatrix();
        glm::vec3 eye = M * glm::vec4(0, 0, 0, 1);
        glm::vec3 center = M * glm::vec4(0, 0, -1, 1);
        glm::vec3 cameraForward = glm::normalize(center - eye);
        std::sort(transparentCommands.begin(), transparentCommands.end(),
                  [cameraForward](const RenderCommand &first, const RenderCommand &second)
                  {
                      // TODO: (Req 9) Finish this function
                      //  HINT: the following return should return true "first" should be drawn before "second".
                      // we need to draw the objecte that is far from the camera and then draw the near to override it
                      // by multiply each object by the forawd vector of the camera, then the far object will get begger value than the near
                      // then the far object appare first in the vector
                      return glm::dot(cameraForward, first.center) > glm::dot(cameraForward, second.center);
                  });

        // TODO: (Req 9) Get the camera ViewProjection matrix and store it in VP
        // get the view matrix and projection matrix from the camera and multiply them both
        glm::mat4 VP = camera->getProjectionMatrix(this->windowSize) * camera->getViewMatrix();
        // p*v *m
        //  TODO: (Req 9) Set the OpenGL viewport using viewportStart and viewportSize
        // the view port start from point 0,0 and set the size in x direction and y direction

        glViewport(0, 0, this->windowSize.x, this->windowSize.y);

        // TODO: (Req 9) Set the clear color to black and the clear depth to 1
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1);
        // the above two lines only setup the clear configuration not the clear it self

        // TODO: (Req 9) Set the color mask to true and the depth mask to true (to ensure the glClear will affect the framebuffer)
        glColorMask(true, true, true, true);
        glDepthMask(true);

        // If there is a postprocess material, bind the framebuffer
        if (postprocessMaterial)
        {
            // TODO: (Req 11) bind the framebuffer
            // here we just need to bind the postprocessFrameBuffer
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postprocessFrameBuffer);
        }

        // TODO: (Req 9) Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // actual clear after setup

        // TODO: (Req 9) Draw all the opaque commands
        //  Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
        // std::cout << "num of color lights : " << lights_list.size() << std::endl;
        for (auto opaqueCommand : opaqueCommands)
        {
            // the VP matrix is still the same in all objects
            // multiply VP with the M matrix of each object wich we get from opaqueCommand.localToWorld
            // then using class matrial to send the MPV matrix to the shader
            // the last step is draw the command using function draw in the mesh , wich draw and swap the buffers and finish the drawing
            glm::mat4 M = opaqueCommand.localToWorld;
            glm::mat4 mpv = VP * M;
            opaqueCommand.material->setup();

            if (dynamic_cast<our::LightMaterial *>(opaqueCommand.material))
            {
                int index = 0;
                for (auto it = lights_list.begin(); it != lights_list.end(); it++, index++)
                {
                    // we need to send all the lights entlightComponenty to the shader
                    // we need to send the data corresponding to each type of light
                    opaqueCommand.material->shader->set("lights[" + std::to_string(index) + "].type", (*it)->lightType);
                    if ((*it)->lightType == 0)
                    {
                        // directional light
                        opaqueCommand.material->shader->set("lights[" + std::to_string(index) + "].direction", (*it)->direction);
                        opaqueCommand.material->shader->set("lights[" + std::to_string(index) + "].color", (*it)->color);
                        // opaqueCommand.material->shader->set("lights[" + std::to_string(index) + "].attenuation", (*it)->attenuation);
                    }
                    else if ((*it)->lightType == 2)
                    {
                        // spot light
                        opaqueCommand.material->shader->set("lights[" + std::to_string(index) + "].position", (*it)->getOwner()->localTransform.position);
                        opaqueCommand.material->shader->set("lights[" + std::to_string(index) + "].direction", (*it)->direction);
                        opaqueCommand.material->shader->set("lights[" + std::to_string(index) + "].color", (*it)->color);
                        opaqueCommand.material->shader->set("lights[" + std::to_string(index) + "].attenuation", (*it)->attenuation);
                        opaqueCommand.material->shader->set("lights[" + std::to_string(index) + "].cone_angles", (*it)->cone_angles);
                    }
                    else
                    {
                        // point
                        auto lightPosition =
                            glm::vec3((*it)->getOwner()->getLocalToWorldMatrix() *
                                      glm::vec4((*it)->getOwner()->localTransform.position, 1.0));
                        opaqueCommand.material->shader->set("lights[" + std::to_string(index) + "].position", lightPosition);
                        opaqueCommand.material->shader->set("lights[" + std::to_string(index) + "].color", (*it)->color);
                        opaqueCommand.material->shader->set("lights[" + std::to_string(index) + "].attenuation", (*it)->attenuation);
                    }
                }
                // std::cout << "num of lightsis : " << (int32_t)lights_list.size() << std::endl;

                opaqueCommand.material->shader->set("light_count", (int32_t)lights_list.size());
                opaqueCommand.material->shader->set("sky.top", glm::vec3(0.1, 0.5, 0.1));
                opaqueCommand.material->shader->set("sky.bottom", glm::vec3(0.1, 0.5, 0.1));
                opaqueCommand.material->shader->set("sky.horizon", glm::vec3(0.1, 0.5, 0.1));

                opaqueCommand.material->shader->set("VP", VP);
                opaqueCommand.material->shader->set("M", opaqueCommand.localToWorld);
                opaqueCommand.material->shader->set("M_IT", glm::transpose(glm::inverse(opaqueCommand.localToWorld)));
                opaqueCommand.material->shader->set("camera_position", eye); // eye * Model of camera
            }
            else
            {
                opaqueCommand.material->shader->set("transform", mpv);
            }
            opaqueCommand.mesh->draw();

            //? here we should send the data to the vshader here
            // send VP and M_IT
            // opaqueCommand.material->shader->set("position", );
        }
        // If there is a sky material, draw the sky
        if (this->skyMaterial)
        {
            // TODO: (Req 10) setup the sky material
            // Setup the sky material
            this->skyMaterial->setup();

            // TODO: (Req 10) Get the camera position
            // Get the camera position
            glm::vec3 cameraPosition = eye;

            // TODO: (Req 10) Create a model matrix for the sy such that it always follows the camera (sky sphere center = camera position)
            // Create a model matrix for the sky such that it always follows the camera (sky sphere center = camera position)
            our::Transform skyTransformObj;
            skyTransformObj.position = cameraPosition;
            glm::mat4 skySphereModel = skyTransformObj.toMat4();

            // TODO: (Req 10) We want the sky to be drawn behind everything (in NDC space, z=1)
            //  We can achieve the is by multiplying by an extra matrix after the projection but what values should we put in it?
            // The matrix should be equal to the following matrix:
            glm::mat4 alwaysBehindTransform = glm::mat4(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 1.0f);

            // TODO: (Req 10) set the "transform" uniform
            // Set the "transform" uniform to the MVP matrix
            this->skyMaterial->shader->set("transform", alwaysBehindTransform * VP * skySphereModel);

            // TODO: (Req 10) draw the sky sphere
            // Draw the sky sphere
            this->skySphere->draw();
        }
        // TODO: (Req 9) Draw all the transparent commands
        //  Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
        for (auto transparentCommand : transparentCommands)
        {
            // the VP matrix is still the same in all objects
            // multiply VP with the M matrix of each object wich we get from transparentCommand.localToWorld
            // then using class matrial to send the MPV matrix to the shader
            // the last step is draw the command using function draw in the mesh , wich draw and swap the buffers and finish the drawing

            glm::mat4 M = transparentCommand.localToWorld;
            glm::mat4 mpv = VP * M;
            transparentCommand.material->setup();
            if (dynamic_cast<our::LightMaterial *>(transparentCommand.material))
            {
                int index = 0;
                for (auto it = lights_list.begin(); it != lights_list.end(); it++, index++)
                {
                    // we need to send all the lights entity to the shader
                    // we need to send the data corresponding to each type of light
                    transparentCommand.material->shader->set("lights[" + std::to_string(index) + "].type", (*it)->lightType);
                    if ((*it)->lightType == 0)
                    {
                        // directional light
                        transparentCommand.material->shader->set("lights[" + std::to_string(index) + "].direction", (*it)->direction);
                        transparentCommand.material->shader->set("lights[" + std::to_string(index) + "].color", (*it)->color);
                    }
                    else if ((*it)->lightType == 1)
                    {
                        // spot light
                        transparentCommand.material->shader->set("lights[" + std::to_string(index) + "].position", (*it)->getOwner()->localTransform.position);
                        transparentCommand.material->shader->set("lights[" + std::to_string(index) + "].direction", (*it)->direction);
                        transparentCommand.material->shader->set("lights[" + std::to_string(index) + "].color", (*it)->color);
                        transparentCommand.material->shader->set("lights[" + std::to_string(index) + "].attenuation", (*it)->attenuation);
                        transparentCommand.material->shader->set("lights[" + std::to_string(index) + "].cone_angles", (*it)->cone_angles);
                    }
                    else
                    {
                        // point light
                        transparentCommand.material->shader->set("lights[" + std::to_string(index) + "].position", (*it)->getOwner()->localTransform.position);
                        transparentCommand.material->shader->set("lights[" + std::to_string(index) + "].color", (*it)->color);
                        transparentCommand.material->shader->set("lights[" + std::to_string(index) + "].attenuation", (*it)->attenuation);
                    }
                }
                // std::cout << "num of lightsis : " << (int32_t)lights_list.size() << std::endl;
                transparentCommand.material->shader->set("light_count", (int32_t)lights_list.size());
                transparentCommand.material->shader->set("sky.top", glm::vec3(0.1, 0.5, 0.1));
                transparentCommand.material->shader->set("sky.bottom", glm::vec3(0.1, 0.5, 0.1));
                transparentCommand.material->shader->set("sky.horizon", glm::vec3(0.1, 0.5, 0.1));

                transparentCommand.material->shader->set("VP", VP);
                transparentCommand.material->shader->set("M", transparentCommand.localToWorld);
                transparentCommand.material->shader->set("M_IT", glm::transpose(glm::inverse(transparentCommand.localToWorld)));
                transparentCommand.material->shader->set("camera_position", eye); // eye * Model of camera
            }
            else
            {
                transparentCommand.material->shader->set("transform", mpv);
            }
            transparentCommand.mesh->draw();
        }
        // If there is a postprocess material, apply postprocessing
        if (postprocessMaterial)
        {
            // TODO: (Req 11) Return to the default framebuffer
            // the default is to unbind the framebuffer
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            glBindVertexArray(postProcessVertexArray);
            // TODO: (Req 11) Setup the postprocess material and draw the fullscreen triangle
            postprocessMaterial->setup();
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
    }

}