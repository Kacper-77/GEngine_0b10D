#pragma once

#include <functional>
#include "Event.h"

class EventManager {
public:
    using EventHandler = std::function<void(const Event&)>;

    void RegisterHandler(const std::string& name, EventHandler handler) {
        handlers[name] = std::move(handler);
    }

    void Emit(const Event& e) {
        auto it = handlers.find(e.name);
        if (it != handlers.end()) {
            it->second(e);
        }
    }

    void Queue(const Event& e) {
        eventQueue.push_back(e);
    }

    void ProcessQueue() {
        for (const auto& e : eventQueue) {
            Emit(e);
        }
        eventQueue.clear();
    }

private:
    std::unordered_map<std::string, EventHandler> handlers;
    std::vector<Event> eventQueue;
};
