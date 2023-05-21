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
    { /**
       * This function prepares the light material for rendering by performing the following steps:
       * 1. Calls the base class `TintedMaterial`'s `setup()` function to perform any necessary setup.
       * 2. Activates texture unit 0 and binds the albedo texture to it.
       * 3. Sets the shader uniform "material.albedo" to texture unit 0.
       * 4. Binds the sampler to texture unit 0.
       * 5. Repeat steps 2-4 for the specular, emissive, roughness, and ambient occlusion textures.
       */
        // Call the setup() function of the base class TintedMaterial
        TintedMaterial::setup();
        // Activate texture unit 0 and bind the albedo texture to it
        glActiveTexture(GL_TEXTURE0);
        albedo->bind();

        // Set the shader uniform "material.albedo" to texture unit 0
        shader->set("material.albedo", 0);

        // Bind the sampler to texture unit 0
        sampler->bind(0);

        // Repeat the above steps for the specular, emissive, roughness, and ambient occlusion textures,
        // using texture units 1, 2, 3, and 4 respectively.

        glActiveTexture(GL_TEXTURE1);
        specular->bind();
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
        // Call the deserialize() function of the base class TintedMaterial
        TintedMaterial::deserialize(data);

        // Check if the JSON data is an object
        if (!data.is_object())
            return;

        // Read the "albedo" value from the JSON object and assign the corresponding texture from the AssetLoader to the member variable
        albedo = AssetLoader<Texture2D>::get(data.value("albedo", ""));

        // Read the "roughness" value from the JSON object and assign the corresponding texture from the AssetLoader to the member variable
        roughness = AssetLoader<Texture2D>::get(data.value("roughness", ""));

        // Read the "specular" value from the JSON object and assign the corresponding texture from the AssetLoader to the member variable
        specular = AssetLoader<Texture2D>::get(data.value("specular", ""));

        // Read the "ambient_occlusion" value from the JSON object and assign the corresponding texture from the AssetLoader to the member variable
        ambient_occlusion = AssetLoader<Texture2D>::get(data.value("ambient_occlusion", ""));

        // Read the "emissive" value from the JSON object and assign the corresponding texture from the AssetLoader to the member variable
        emissive = AssetLoader<Texture2D>::get(data.value("emissive", ""));

        // Read the "sampler" value from the JSON object and assign the corresponding sampler from the AssetLoader to the member variable
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

}