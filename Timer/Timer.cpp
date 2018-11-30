#include <stdio.h>
#include <sys/time.h>

#include "Timer.h"

Timer::Timer() {
    this->Total_time = 0.0;
}

Timer::~Timer() {
    
}
void Timer::Start_Timer() {
    gettimeofday(&start_time, NULL);
}
void Timer::End_Timer() {
    gettimeofday(&end_time, NULL);
}
void Timer::CalculateElapseTime() {
    
    float ElapseTime;
    
    ElapseTime = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    this->Total_time = ElapseTime;
}

void Timer::print()
{
    printf("Elapse Time (min): %f\n", this->Total_time / 60.0);
}
