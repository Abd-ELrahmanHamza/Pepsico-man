#include "mesh-renderer.hpp"
#include "../asset-loader.hpp"

namespace our
{
    // Receives the mesh & material from the AssetLoader by the names given in the json object
    void MeshRendererComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        // Notice how we just get a string from the json file and pass it to the AssetLoader to get us the actual asset
        //(Req 8) Get the material and the mesh from the AssetLoader by their names
        // which are defined with the keys "mesh" and "material" in data.
        // Hint: To get a value of type T from a json object "data" where the key corresponding to the value is "key",
        // you can use write: data["key"].get<T>().
        // Look at "source/common/asset-loader.hpp" to know how to use the static class AssetLoader.
        // AssetLoader<Mesh>* meshLoader = new AssetLoader<Mesh>();
        // AssetLoader<Mesh>* meshLoader = new AssetLoader<Mesh>();
        // AssetLoader<Mesh>::deserialize(data["mesh"]);
        // AssetLoader<Material>::deserialize(data["material"]);

        mesh = AssetLoader<Mesh>::get(data["mesh"].get<std::string>());             // get the mesh from the asset loader
        material = AssetLoader<Material>::get(data["material"].get<std::string>()); // get the material from the asset loader
    }
}