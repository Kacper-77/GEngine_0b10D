#pragma once

#include <unordered_map>
#include <vector>
#include <queue>
#include <functional>
#include <typeindex>
#include <memory>
#include <shared_mutex> 
#include <cassert>
#include <cstdint>
#include <iostream>

#include "Event.h"

// Cosmetics
using HandlerID = std::uint64_t;

template<typename T>
using EventHandler = std::function<void(const T&)>;

class EventBus {
public:
    // Subscribe event
    template<typename T>
    HandlerID Subscribe(EventHandler<T> handler) {
        const std::type_index type = typeid(T);  // Get type index

        // Check types and valid them
        auto wrapper = [handler](const Event& e) {
            assert(typeid(e) == typeid(T) && "Event type mismatch in handler.");
            handler(static_cast<const T&>(e));
        };

        // Thread safety (deadlock, race conditon etc.)
        std::unique_lock<std::shared_mutex> lock(mutex_);

        HandlerID id = nextHandlerID_++;
        listeners[type].emplace_back(id, std::move(wrapper));
        return id;
    }

    // Unsubscribe event
    template<typename T>
    void Unsubscribe(HandlerID id) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        std::type_index type = typeid(T);
        bool unsubscribed = false;

        auto it = listeners.find(type);
        if (it != listeners.end()) {
            auto& vec = it->second;
            vec.erase(std::remove_if(vec.begin(), vec.end(),
                      [id](const auto& pair) { return pair.first == id; }),
                      vec.end());
            unsubscribed = true;
        }
        if (!unsubscribed) {
            std::cerr << "Something went wrong / Event not found." << std::endl;
        }
    }

    // Add event to queue
    template<typename T>
    void Publish(const T& event) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        eventQueue.emplace(std::make_unique<T>(event));
    }

    // Call event immediately
    template<typename T>
    void PublishImmediate(const T& event) {
        const std::type_index type = typeid(T);

        // To avoid undefined behaviors
        std::vector<EventHandler<T>> handlersCopy;

        {
            // Thread safety
            std::shared_lock<std::shared_mutex> lock(mutex_);

            auto it = listeners.find(type);
            if (it != listeners.end()) {
                for (auto& [id, func] : it->second) {
                    handlersCopy.push_back(func);
                }
            }
        }

        // Calling event
        for (auto& handler : handlersCopy) {
            handler(event);
        }
    }

    // Call all events from Queue
    void Dispatch() {
        // To avoid undefined behaviors pt. 1
        std::queue<std::unique_ptr<Event>> localQueue;

        {
            std::shared_lock<std::shared_mutex> lock(mutex_);

            // To avoid undefined behaviors pt. 2
            std::swap(eventQueue, localQueue);
        }

        while (!localQueue.empty()) {
            // Get and remove event from local queue
            std::unique_ptr<Event> event = std::move(localQueue.front());
            localQueue.pop();

            // Type of event
            const std::type_index type = typeid(*event);
            // To avoid undefined behaviors pt. 3
            std::vector<EventHandler<const Event&>> handlersCopy;

            {
                std::shared_lock<std::shared_mutex> lock(mutex_);
                // Get registered event and insert in handlersCopy
                auto it = listeners.find(type);
                if (it != listeners.end()) {
                    for (auto& [id, func] : it->second) {
                        handlersCopy.push_back(func);
                    }
                }
            }

            // Dispatch all events
            for (auto& handler : handlersCopy) {
                handler(*event);
            }
        }
    }

    // Cleanup
    void Clear() {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        listeners.clear();
        std::queue<std::unique_ptr<Event>> empty;
        std::swap(eventQueue, empty);
    }

    // Get number of queued events
    size_t QueuedEventCount() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return eventQueue.size();
    }

    // Get number of registered events by type
    template<typename T>
    size_t ListenerCount() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        std::type_index type = typeid(T);

        auto it = listeners.find(type);
        return it != listeners.end() ? it->second.size() : 0;
    }

private:
    std::unordered_map<std::type_index,
                       std::vector<std::pair<HandlerID, 
                       EventHandler<const Event&>>>> listeners;

    std::queue<std::unique_ptr<Event>> eventQueue;
    mutable std::shared_mutex mutex_;  // Thread-Safe, shared in future
    HandlerID nextHandlerID_ = 1;
};
