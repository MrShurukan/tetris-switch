#include "timer.hpp"

Timer::Timer(float timeSeconds) 
    : timeSeconds(timeSeconds), 
      timeLeft(timeSeconds), 
      callback(nullptr) {}

Timer::Timer(float timeSeconds, std::function<void()> callback) 
    : timeSeconds(timeSeconds), 
      timeLeft(timeSeconds), 
      callback(callback) {}

bool Timer::process(float deltaTime) {
    bool hasTimeElapsed = false;

    this->timeLeft -= deltaTime;
    if (this->timeLeft <= 0.0f) {
        hasTimeElapsed = true;
        this->timeLeft += this->timeSeconds;
        if (this->callback) this->callback();
    }

    return hasTimeElapsed;
}

void Timer::reset() {
    this->timeLeft = this->timeSeconds;
}

void Timer::reset(float timeSeconds) {
    this->timeSeconds = timeSeconds;
    this->reset();
}

void Timer::trigger() {
    this->timeLeft = 0.0f;
}

void Timer::triggerAndReset(float timeSeconds) {
    this->reset(timeSeconds);
    this->trigger();
};

float Timer::getTimeLeft() const {
    return this->timeLeft;
}