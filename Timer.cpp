#include "Timer.h"

Timer::Timer() : timeLimit(0), currentTime(0) {}

void Timer::SetTimeLimit(float seconds) {
    timeLimit = seconds;
    currentTime = seconds;
}

void Timer::AddTime(float seconds) {
    currentTime += seconds;
    if (currentTime > timeLimit) {
        currentTime = timeLimit;
    }
}

void Timer::SubtractTime(float seconds) {
    currentTime -= seconds;
    if (currentTime < 0) {
        currentTime = 0;
    }
}

float Timer::GetCurrentTime() const {
    return currentTime;
}

float Timer::GetTimeLimit() const {
    return timeLimit;
}

bool Timer::IsTimeUp() const
{
    if (currentTime <= 0) {
		return true;
	}

    return false;
}

void Timer::Update() {
    sf::Time dt = clock.restart();
    currentTime -= dt.asSeconds();
    if (currentTime < 0) {
        currentTime = 0;
    }
}

void Timer::DisplayTime() {
    int roundedTime = static_cast<int>(std::round(currentTime));
    int roundedLimit = static_cast<int>(std::round(timeLimit));
    if (roundedTime != lastDisplayedTime) {
        std::cout << "Time: " << roundedTime << "s / " << roundedLimit << "s" << std::endl;
        lastDisplayedTime = roundedTime;
    }
}
