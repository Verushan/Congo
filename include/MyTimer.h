#pragma once

#include <chrono>

class MyTimer {
public:
    MyTimer();

    void startTimer();

    long long getElapsedTime();

private:
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::_V2::system_clock::rep elapsedTime;
};