#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>

/**
 * High-precision stopwatch for measuring execution time.
 * All get_* methods return values accurate to the nanosecond.
 */
class Stopwatch {
public:
    Stopwatch();

    void start();   // starts the timer
    void stop();    // stops the timer
    void reset();   // sets stopwatch to zero

    double get_minutes();      // number of minutes counted, as a double
    double get_seconds();      // number of seconds counted, as a double
    double get_milliseconds(); // number of milliseconds counted, as a double
    double get_nanoseconds();  // number of nanoseconds counted, as a double

private:
    using clock = std::chrono::high_resolution_clock;
    using nanosecond = std::chrono::nanoseconds;

    bool running_;
    nanosecond elapsed_;                    // total elapsed time when stopped
    std::chrono::time_point<clock> start_;  // start time when running
};

#endif // STOPWATCH_H
