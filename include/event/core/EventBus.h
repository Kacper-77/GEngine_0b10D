#pragma once

#include <unordered_map>
#include <vector>
#include <queue>
#include <functional>
#include <typeindex>
#include <memory>
#include <mutex>

#include "Event.h"

class EventBus {
public:
    EventBus() = default;

    template<typename T>
    void Subscribe(std::function<void(const T&)> handler) {
        const std::type_index type = typeid(T);
        auto wrapper = [handler](const Event& e) {
            handler(static_cast<const T&>(e));
        };
        std::lock_guard<std::mutex> lock(mutex_);
        listeners[type].emplace_back(std::move(wrapper));
    }

    template<typename T>
    void Queue(const T& event) {
        std::lock_guard<std::mutex> lock(mutex_);
        eventQueue.emplace(std::make_unique<T>(event));
    }

    void Dispatch() {
        std::lock_guard<std::mutex> lock(mutex_);
        while (!eventQueue.empty()) {
            std::unique_ptr<Event> event = std::move(eventQueue.front());
            eventQueue.pop();
            
            const std::type_index type = typeid(*event);
            auto it = listeners.find(type);
            if (it != listeners.end()) {
                for (auto& handler : it->second) {
                    handler(*event);
                }
            }
        }
    }

    void Clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        listeners.clear();
        std::queue<std::unique_ptr<Event>> empty;
        std::swap(eventQueue, empty);
    }

private:
    std::unordered_map<std::type_index, std::vector<std::function<void(const Event&)>>> listeners;
    std::queue<std::unique_ptr<Event>> eventQueue;
    std::mutex mutex_;
};
