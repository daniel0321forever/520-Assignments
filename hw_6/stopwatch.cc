#include "stopwatch.h"

Stopwatch::Stopwatch() : running_(false), elapsed_(0) {}

void Stopwatch::start() {
    if (!running_) {
        start_ = clock::now();
        running_ = true;
    }
}

void Stopwatch::stop() {
    if (running_) {
        elapsed_ += std::chrono::duration_cast<nanosecond>(clock::now() - start_);
        running_ = false;
    }
}

void Stopwatch::reset() {
    elapsed_ = nanosecond(0);
    running_ = false;
}

double Stopwatch::get_nanoseconds() {
    nanosecond total = elapsed_;
    if (running_) {
        total += std::chrono::duration_cast<nanosecond>(clock::now() - start_);
    }
    return static_cast<double>(total.count());
}

double Stopwatch::get_milliseconds() {
    return get_nanoseconds() / 1e6;
}

double Stopwatch::get_seconds() {
    return get_nanoseconds() / 1e9;
}

double Stopwatch::get_minutes() {
    return get_nanoseconds() / 6e10;
}
