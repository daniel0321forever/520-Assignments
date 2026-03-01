#include "integrator.h"

namespace elma {

Integrator::Integrator(std::string name) : Process(name), integral_(0.0) {}

void Integrator::init() {
    integral_ = 0.0;
}

void Integrator::update() {
    Channel& ch = channel("link");
    if (ch.nonempty()) {
        double v = ch.latest().get<double>();
        integral_ += delta() * v;
    }
}

} // namespace elma
