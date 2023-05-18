#include "heart.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {

    void HeartComponent::deserialize(const nlohmann::json &data) {
        if (!data.is_object()) return;
        heartNumber = data.value("number", heartNumber);
    }
}