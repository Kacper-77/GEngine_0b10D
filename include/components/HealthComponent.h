#pragma once

struct HealthComponent {
    int currentHP = 100;
    int maxHP = 100;        
    bool isDead = false;
    int shield = 0;      
    float regenRate = 0.0f;
    bool invulnerable = false;

    HealthComponent(int health, int maxHealth)
        : currentHP{health}, maxHP{maxHealth} {}
};