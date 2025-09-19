#ifndef COMMON_TIMER_
#define COMMON_TIMER_

#include <chrono>

#include <sys/time.h>
#include <sys/select.h>
#include <stdio.h>

class Timer
{
public:
    void Tic() { ts = std::chrono::high_resolution_clock::now(); }
    void Toc() { te = std::chrono::high_resolution_clock::now(); }
    double msDuration() { return std::chrono::duration<double, std::milli>(te - ts).count(); }
    double usDuration() { return std::chrono::duration<double, std::micro>(te - ts).count(); }
    double nsDuration() { return std::chrono::duration<double, std::nano>(te - ts).count(); }
    void msDelay(int time)
    {
        Tic();
        while (Toc(), msDuration() < time) {}
    }

    void usDelay(int time)
    {
        temp.tv_sec = 0;
        temp.tv_usec = time;
        select(0, NULL, NULL, NULL, &temp);
        return;
    }

    void nsDelay(int time)
    {
        Tic();
        while (Toc(), usDuration() < time) {}
    }

    std::time_t GetTimeStamp()
    {
        std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
        std::time_t timestamp = tp.time_since_epoch().count();
        return timestamp;
    }

private:
    std::chrono::_V2::system_clock::time_point ts;
    std::chrono::_V2::system_clock::time_point te;
    double duration;

    struct timeval temp;
};

#endif  // COMMON_TIMER_
