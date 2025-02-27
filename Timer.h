#ifndef TIMER_H
#define TIMER_H
#include <SFML/System.hpp>
#include <iostream>

class Timer 
{
public:
    Timer();

    void SetTimeLimit(float seconds);
    void AddTime(float seconds);
    void SubtractTime(float seconds);
    void Update();
    void DisplayTime();

    float GetCurrentTime() const;
    float GetTimeLimit() const;

    bool IsTimeUp() const;
  
private:
    sf::Clock clock;
    float timeLimit;
    float currentTime;

    int lastDisplayedTime;
};
#endif // TIMER_H
