#pragma once

struct VectorInt {
    int x, y;
};

struct VectorFloat {
    float x, y;

    VectorFloat() : x{0}, y{0} {}
    VectorFloat(float x, float y) : x{x}, y{y} {}

    VectorFloat operator+(const VectorFloat& other) const {
        return {x + other.x, y + other.y};
    }

    VectorFloat operator-(const VectorFloat& other) const {
        return {x - other.x, y - other.y};
    }

    VectorFloat operator*(float scalar) const {
        return {x * scalar, y * scalar};
    }

    float Length() const {
        return std::sqrt(x*x + y*y);
    }

    VectorFloat Normalized() const {
        float len = Length();
        return len > 0 ? VectorFloat{x/len, y/len} : VectorFloat{};
    }
};
