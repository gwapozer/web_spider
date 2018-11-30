#ifndef _TIMER_H
#define	_TIMER_H

class Timer
{
public:
    Timer();
    ~Timer();
    
    void Start_Timer();
    void End_Timer();
    void CalculateElapseTime();
    
    void print();

private:
    struct timeval start_time;
    struct timeval end_time;
    
    float Total_time;
};

#endif	/* _TIMER_H */

