#pragma once
#include <functional>
#include <stdbool.h>
#include <stddef.h>

// General purpose periodic timer
class Timer {
public:
    Timer(float timeSeconds);
    Timer(float timeSeconds, std::function<void()> callback);
    // Ticks the timer, calls the callback if exists. Returns true once timer hits, then resets
    bool process(float deltaTime);
    void reset();
    // Resets timer and sets new timeSeconds for the next run
    void reset(float timeSeconds);
    // Next time process will be called will cause timer to trigger as if it reached 0. Doesn't call process itself.
    void trigger();
    // Triggers the timer, then resets to a new timeSeconds. See trigger() comment.
    void triggerAndReset(float timeSeconds);
    // Gets how much time left in seconds
    float getTimeLeft() const;
private:
    // How often should it fire
    float timeSeconds;
    // Actual timer variable
    float timeLeft;
    // Optional callback (otherwise you just work off the processTimer return value)
    std::function<void()> callback;
};