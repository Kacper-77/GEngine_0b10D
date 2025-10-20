#pragma once

#include <string>
#include <unordered_map>

// Unused now but can be implemented in future
class EventResolver {
public:
    virtual ~EventResolver() = default;
    virtual std::string ResolveEventType(const std::string& behaviorA, 
                                         const std::string& behaviorB) const = 0;
};