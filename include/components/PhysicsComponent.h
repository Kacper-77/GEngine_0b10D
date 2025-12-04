#pragma once

struct PhysicsComponent {
    float mass = 1.0f;
    float invMass = 1.0f; // 1/mass (0 for infinite mass)

    VectorFloat velocity{0, 0};
    VectorFloat force{0, 0};      // sum of forces this frame
    VectorFloat impulse{0, 0};    // instantaneous impulses

    float gravityScale = 1.0f;

    float frictionStatic = 0.6f;
    float frictionKinetic = 0.4f;

    float linearDamping = 0.02f; // drag

    float maxSpeed = 500.0f;

    bool isGrounded = false;

    void SetMass(float m) {
        mass = m;
        invMass = (m == 0.0f ? 0.0f : 1.0f / m);
    }
};
