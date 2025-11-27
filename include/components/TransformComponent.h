#pragma once

#include "utils/Vector.h"

struct TransformComponent {
    VectorFloat position {0.0f, 0.0f};
    float rotationDeg = 0.0f;
    VectorFloat scale {1.0f, 1.0f};
};
