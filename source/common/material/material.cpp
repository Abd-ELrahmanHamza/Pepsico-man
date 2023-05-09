#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

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
        Material::setup();

        glActiveTexture(GL_TEXTURE0); // activate texture unit 0
        texture->bind();              // bind texture to texture2D
        if (sampler)
            sampler->bind(0);
        //! h7ot asamy el objects material.albedo for example.
        // shader->set("tex", 0); // set uniform of tex to texture unit 0
        // shader->set("tex", 0); // set uniform of tex to texture unit 0
        // shader->set("tex", 0); // set uniform of tex to texture unit 0
        // shader->set("tex", 0); // set uniform of tex to texture unit 0
        // shader->set("tex", 0); // set uniform of tex to texture unit 0

        glActiveTexture(GL_TEXTURE1); // activate texture unit 0
        texture->bind();              // bind texture to texture2D
        if (sampler)
            sampler->bind(1);
        // shader->set("tex", 0); // set uniform of tex to texture unit 0
        // shader->set("tex", 0); // set uniform of tex to texture unit 0
        // shader->set("tex", 0); // set uniform of tex to texture unit 0
        // shader->set("tex", 0); // set uniform of tex to texture unit 0
        // shader->set("tex", 0); // set uniform of tex to texture unit 0

        glActiveTexture(GL_TEXTURE2); // activate texture unit 0
        texture->bind();              // bind texture to texture2D
        if (sampler)
            sampler->bind(2);
        // shader->set("tex", 0); // set uniform of tex to texture unit 0
        // shader->set("tex", 0); // set uniform of tex to texture unit 0
        // shader->set("tex", 0); // set uniform of tex to texture unit 0
        // shader->set("tex", 0); // set uniform of tex to texture unit 0
        // shader->set("tex", 0); // set uniform of tex to texture unit 0

        glActiveTexture(GL_TEXTURE3); // activate texture unit 0
        texture->bind();              // bind texture to texture2D
        if (sampler)
            sampler->bind(3);
        // shader->set("tex", 0); // set uniform of tex to texture unit 0
        // shader->set("tex", 0); // set uniform of tex to texture unit 0
        // shader->set("tex", 0); // set uniform of tex to texture unit 0
        // shader->set("tex", 0); // set uniform of tex to texture unit 0
        // shader->set("tex", 0); // set uniform of tex to texture unit 0

        glActiveTexture(GL_TEXTURE4); // activate texture unit 0
        texture->bind();              // bind texture to texture2D
        if (sampler)
            sampler->bind(4);
        // shader->set("tex", 0); // set uniform of tex to texture unit 0
        // shader->set("tex", 0); // set uniform of tex to texture unit 0
        // shader->set("tex", 0); // set uniform of tex to texture unit 0
        // shader->set("tex", 0); // set uniform of tex to texture unit 0
        // shader->set("tex", 0); // set uniform of tex to texture unit 0
    }

}