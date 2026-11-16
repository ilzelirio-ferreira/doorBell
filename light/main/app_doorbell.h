#pragma once
#include <stdint.h>

// Polled every 10 ms. A contact closes to GND; true means pressed.
struct AppDoorbell {
    static constexpr int64_t debounce_us = 30000;
    static constexpr int64_t duration_us = 10000000;
    static constexpr int64_t half_period_us = 500000;
    bool candidate = false;
    bool stable = false;
    bool active = false;
    bool phase = false;
    int64_t candidate_since = 0;
    int64_t started = 0;

    bool sample(bool pressed, int64_t now) {
        if (pressed != candidate) {
            candidate = pressed;
            candidate_since = now;
        }
        if (candidate == stable || now - candidate_since < debounce_us) return false;
        stable = candidate;
        if (!stable) return false;
        // A fresh press restarts the full indication interval.
        active = phase = true;
        started = now;
        return true;
    }
    void tick(int64_t now) {
        if (!active) return;
        const int64_t elapsed = now - started;
        if (elapsed >= duration_us) { active = phase = false; return; }
        phase = (elapsed / half_period_us) % 2 == 0;
    }
};
