#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "elma.h"

namespace elma {

/**
 * Process that numerically integrates values read from the "link" channel.
 * integral += delta() * value each update. Initial value is zero.
 */
class Integrator : public Process {
public:
    Integrator(std::string name);

    void init() override;
    void start() override {}
    void update() override;
    void stop() override {}

    /// Returns the current integral value.
    double value() const { return integral_; }

private:
    double integral_;
};

} // namespace elma

#endif // INTEGRATOR_H
