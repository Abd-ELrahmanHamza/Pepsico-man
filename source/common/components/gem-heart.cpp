#include "gem-heart.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {

    void GemHeartComponent::deserialize(const nlohmann::json &data) {
        if (!data.is_object()) return;
    }
}