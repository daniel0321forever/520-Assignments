#include "random_process.h"

namespace elma {

RandomProcess::RandomProcess(std::string name)
    : Process(name), gen_(std::random_device{}()), dist_(0.0, 1.0) {}

void RandomProcess::update() {
    double value = dist_(gen_);
    channel("link").send(nlohmann::json(value));
}

} // namespace elma
