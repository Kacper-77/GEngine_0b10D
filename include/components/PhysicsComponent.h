#pragma once

struct PhysicsComponent {
    float posX = 0.0f;
    float posY = 0.0f;
    float velocityX = 0.0f;
    float velocityY = 0.0f;
    float friction = 0.0f;          
    float gravityScale = 1.0f;      
    float mass = 1.0f;              
    float bounceFactor = 0.0f;      
    float maxVelocityX = 1000.0f;   
    float maxVelocityY = 1000.0f;
    bool isGrounded = false;
};

