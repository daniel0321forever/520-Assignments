#ifndef FILTER_H
#define FILTER_H

#include "elma.h"
#include <deque>

namespace elma {

/**
 * Process that reads from "link" channel and maintains a running average
 * of the last 10 values (or fewer if not yet 10 received). Initial average is 0.
 */
class Filter : public Process {
public:
    Filter(std::string name);

    void init() override;
    void start() override {}
    void update() override;
    void stop() override {}

    /// Returns the current running average.
    double value() const { return running_avg_; }

private:
    static const size_t WINDOW_SIZE = 10;
    std::deque<double> window_;
    double running_avg_;
};

} // namespace elma

#endif // FILTER_H
