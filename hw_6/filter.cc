#include "filter.h"

namespace elma {

Filter::Filter(std::string name) : Process(name), running_avg_(0.0) {}

void Filter::init() {
    window_.clear();
    running_avg_ = 0.0;
}

void Filter::update() {
    Channel& ch = channel("link");
    if (ch.nonempty()) {
        double v = ch.latest().get<double>();
        if (window_.size() >= WINDOW_SIZE) {
            double old = window_.front();
            window_.pop_front();
            running_avg_ += (v - old) / WINDOW_SIZE;
        } else {
            size_t n = window_.size() + 1;
            running_avg_ = (running_avg_ * window_.size() + v) / n;
        }
        window_.push_back(v);
    }
}

} // namespace elma
