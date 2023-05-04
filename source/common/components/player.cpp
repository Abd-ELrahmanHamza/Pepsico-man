#include "player.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    // Reads linearVelocity & angularVelocity from the given json object
    void PlayerComponent::deserialize(const nlohmann::json &data) {
        if (!data.is_object()) return;
        this->speed = data.value("speed", this->speed);
    }
}