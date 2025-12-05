#pragma once

#include <vector>
#include "graphics/Texture.h"

struct BackgroundLayer {
    int texW;
    int texH;
    Texture* texture;
    float parallaxFactor = 0.0f;  /*
                                    0.0f - static,
                                    0.03 - slow,
                                    0.06 - medium,
                                    1.0f - moves with camera,
                                    > 1.0f - faster than camera
                                  */
                                 
};
