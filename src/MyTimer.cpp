#include "MyTimer.h"

MyTimer::MyTimer() {
    start = std::chrono::high_resolution_clock::now();
    elapsedTime = 0;
}

void MyTimer::startTimer() {
    start = std::chrono::high_resolution_clock::now();
}

long long MyTimer::getElapsedTime() {
    auto end = std::chrono::high_resolution_clock::now();
    elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    return elapsedTime;
}