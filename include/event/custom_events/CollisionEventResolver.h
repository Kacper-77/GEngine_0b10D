#pragma once

#include "utils/EventResolver.h"
#include <unordered_map>
#include <string>

class CollisionEventResolver : public EventResolver {
public:
    std::string ResolveEventType(const std::string& behaviorA, 
                                 const std::string& behaviorB) const override {
        static const std::unordered_map<std::string, std::string> behaviorToEvent = {
            {"sticky", "Stick"},
            {"explosive", "Damage"},
            {"slime", "Damage"},
            {"bouncy", "Bounce"},
            {"ghost", "PassThrough"},
            {"magnetic", "Pull"},
            {"repellent", "Push"}
        };

        for (const auto& behavior : {behaviorA, behaviorB}) {
            auto it = behaviorToEvent.find(behavior);
            if (it != behaviorToEvent.end()) {
                return it->second;
            }
        }
        return "Nothing";
    }
};
