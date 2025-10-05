#pragma once

#include <unordered_map>
#include <vector>
#include <queue>
#include <functional>
#include <typeindex>
#include <memory>
#include <mutex>
#include <cassert>
#include <cstdint>

#include "Event.h"

class EventBus {
public:
    using HandlerID = std::uint64_t;

    EventBus() = default;

    template<typename T>
    HandlerID Subscribe(std::function<void(const T&)> handler) {
        const std::type_index type = typeid(T);

        auto wrapper = [handler](const Event& e) {
            assert(typeid(e) == typeid(T) && "Event type mismatch in handler.");
            handler(static_cast<const T&>(e));
        };

        std::lock_guard<std::mutex> lock(mutex_);
        HandlerID id = nextHandlerId_++;
        listeners[type].emplace_back(id, std::move(wrapper));
        return id;
    }

    bool Unsubscribe(std::type_index type, HandlerID id) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = listeners.find(type);
        if (it != listeners.end()) {
            auto& vec = it->second;
            vec.erase(std::remove_if(vec.begin(), vec.end(),
                      [id](const auto& pair) { return pair.first == id; }),
                      vec.end());
            return true;
        }
        return false;
    }

    template<typename T>
    void Publish(const T& event) {
        std::lock_guard<std::mutex> lock(mutex_);
        eventQueue.emplace(std::make_unique<T>(event));
    }

    template<typename T>
    void PublishImmediate(const T& event) {
        const std::type_index type = typeid(T);
        std::vector<std::function<void(const Event&)>> handlersCopy;

        {
            std::lock_guard<std::mutex> lock(mutex_);
            auto it = listeners.find(type);
            if (it != listeners.end()) {
                for (auto& [id, func] : it->second) {
                    handlersCopy.push_back(func);
                }
            }
        }

        for (auto& handler : handlersCopy) {
            handler(event);
        }
    }

    void Dispatch() {
        std::queue<std::unique_ptr<Event>> localQueue;

        {
            std::lock_guard<std::mutex> lock(mutex_);
            std::swap(eventQueue, localQueue);
        }

        while (!localQueue.empty()) {
            std::unique_ptr<Event> event = std::move(localQueue.front());
            localQueue.pop();

            const std::type_index type = typeid(*event);
            std::vector<std::function<void(const Event&)>> handlersCopy;

            {
                std::lock_guard<std::mutex> lock(mutex_);
                auto it = listeners.find(type);
                if (it != listeners.end()) {
                    for (auto& [id, func] : it->second) {
                        handlersCopy.push_back(func);
                    }
                }
            }

            for (auto& handler : handlersCopy) {
                handler(*event);
            }
        }
    }

    void Clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        listeners.clear();
        std::queue<std::unique_ptr<Event>> empty;
        std::swap(eventQueue, empty);
    }

    size_t QueuedEventCount() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return eventQueue.size();
    }

    size_t ListenerCount(std::type_index type) const {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = listeners.find(type);
        return it != listeners.end() ? it->second.size() : 0;
    }

private:
    std::unordered_map<std::type_index,
        std::vector<std::pair<HandlerID, std::function<void(const Event&)>>>> listeners;

    std::queue<std::unique_ptr<Event>> eventQueue;
    mutable std::mutex mutex_;
    HandlerID nextHandlerId_ = 1;
};
