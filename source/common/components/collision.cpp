#include "collision.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    // Reads linearVelocity & angularVelocity from the given json object
    void CollisionComponent::deserialize(const nlohmann::json &data) {
        if (!data.is_object()) return;
        start = data.value("start", start);
        end = data.value("end", end);
    }
}