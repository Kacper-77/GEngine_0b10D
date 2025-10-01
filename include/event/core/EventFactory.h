#pragma once

#include <unordered_map>
#include "Event.h"

class EventFactory {
public:
    void RegisterTemplate(const std::string& name, const std::unordered_map<std::string, float>& data) {
        templates[name] = data;
    }

    Event Create(const std::string& name, EntityID source, EntityID target) const {
        auto it = templates.find(name);
        if (it != templates.end()) {
            return { name, source, target, it->second };
        }
        return { name, source, target, {} }; // fallback
    }

private:
    std::unordered_map<std::string, std::unordered_map<std::string, float>> templates;
};
