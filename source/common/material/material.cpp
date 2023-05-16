#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"
#include <iostream>
namespace our
{

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const
    {
        // (Req 7) Write this function
        pipelineState.setup(); // call setup of pipeline state
        // should i create a shader or link it
        // shader = new ShaderProgram();
        shader->use(); // call use of shader
        // shader->link();  // no need to link shader
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;

        if (data.contains("pipelineState"))
        {
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint
    void TintedMaterial::setup() const
    {
        // (Req 7) Write this function
        Material::setup();         // call setup of Material "parent"
        shader->set("tint", tint); // set uniform of tint
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json &data)
    {
        Material::deserialize(data);
        if (!data.is_object())
            return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex"
    void TexturedMaterial::setup() const
    {
        // (Req 7) Write this function
        // call setup TintedMaterial
        TintedMaterial::setup(); // call setup of TintedMaterial "parent"

        shader->set("alphaThreshold", alphaThreshold); // set uniform of alphaThreshold

        // should i activate texture unit ??
        // should texture unit be zero ?

        glActiveTexture(GL_TEXTURE0); // activate texture unit 0

        texture->bind();        // bind texture to texture2D
        if (sampler != nullptr) // check if sampler is not null
            sampler->bind(0);   // bind sampler to texture unit 0

        shader->set("tex", 0); // set uniform of tex to texture unit 0
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json &data)
    {
        TintedMaterial::deserialize(data);
        if (!data.is_object())
            return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

    void LightMaterial::setup() const
    {
        TintedMaterial::setup();
        glActiveTexture(GL_TEXTURE0); // activate texture unit 0
        albedo->bind();               // bind texture to texture2D
        shader->set("material.albedo", 0);
        sampler->bind(0);

        glActiveTexture(GL_TEXTURE1); // activate texture unit 0
        specular->bind();             // bind texture to texture2D
        shader->set("material.specular", 1);
        sampler->bind(1);

        glActiveTexture(GL_TEXTURE2);
        emissive->bind();
        shader->set("material.emissive", 2);
        sampler->bind(2);

        glActiveTexture(GL_TEXTURE3);
        roughness->bind();
        shader->set("material.roughness", 3);
        sampler->bind(3);

        glActiveTexture(GL_TEXTURE4);
        ambient_occlusion->bind();
        shader->set("material.ambient_occlusion", 4);
        sampler->bind(4);

        //
    }

    void LightMaterial::deserialize(const nlohmann::json &data)
    {
        TintedMaterial::deserialize(data);
        if (!data.is_object())
            return;
        albedo = AssetLoader<Texture2D>::get(data.value("albedo", ""));
        roughness = AssetLoader<Texture2D>::get(data.value("roughness", ""));
        specular = AssetLoader<Texture2D>::get(data.value("specular", ""));
        ambient_occlusion = AssetLoader<Texture2D>::get(data.value("ambient_occlusion", ""));
        emissive = AssetLoader<Texture2D>::get(data.value("emissive", ""));

        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

}