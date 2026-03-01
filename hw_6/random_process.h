#ifndef RANDOM_PROCESS_H
#define RANDOM_PROCESS_H

#include "elma.h"
#include <random>

namespace elma {

/**
 * Process that sends random doubles in [0, 1] to the "link" channel every update.
 */
class RandomProcess : public Process {
public:
    RandomProcess(std::string name);

    void init() override {}
    void start() override {}
    void update() override;
    void stop() override {}

private:
    std::mt19937 gen_;
    std::uniform_real_distribution<double> dist_;
};

} // namespace elma

#endif // RANDOM_PROCESS_H
