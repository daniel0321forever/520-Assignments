// HW6 Part 1: Stopwatch demo and Part 2: ELMA process composition
#include <iostream>
#include <chrono>
#include <thread>

#include "stopwatch.h"
#include "elma.h"
#include "random_process.h"
#include "filter.h"
#include "integrator.h"

#define SLEEP std::this_thread::sleep_for(std::chrono::milliseconds(1500))

using namespace std::chrono;
using namespace elma;

// Simple process that sends a constant value to "link" every update (for demo only)
class ConstantProcess : public Process {
public:
    ConstantProcess(std::string name, double value)
        : Process(name), value_(value) {}
    void init() override {}
    void start() override {}
    void update() override { channel("link").send(nlohmann::json(value_)); }
    void stop() override {}

private:
    double value_;
};

int main() {
    // Part 1: Stopwatch example from assignment
    std::cout << "=== Part 1: Stopwatch ===" << std::endl;
    Stopwatch w;
    w.start();
    SLEEP;
    w.stop();
    std::cout << w.get_seconds() << "\n";  // about 1.5
    SLEEP;
    std::cout << w.get_seconds() << "\n";  // still about 1.5
    w.start();
    SLEEP;
    w.stop();
    std::cout << w.get_seconds() << "\n";  // about 3.0
    w.reset();
    std::cout << w.get_seconds() << "\n";  // 0.0

    // Part 2: RandomProcess + Filter
    std::cout << "\n=== Part 2: RandomProcess + Filter ===" << std::endl;
    Manager m;
    RandomProcess r("random numbers");
    Filter f("filter");
    Channel link("link");

    m.schedule(r, 1_ms)
        .schedule(f, 1_ms)
        .add_channel(link)
        .init()
        .run(100_ms);

    std::cout << "After 100ms, filter value (running avg of last 10): " << f.value() << std::endl;

    // Part 2: Integrator with constant source
    std::cout << "\n=== Part 2: Integrator (constant 1.0) ===" << std::endl;
    Manager m2;
    ConstantProcess src("source", 1.0);
    Integrator integ("integrator");
    Channel link2("link");
    m2.schedule(src, 1_ms)
        .schedule(integ, 1_ms)
        .add_channel(link2)
        .init()
        .run(10_ms);
    std::cout << "Integrator value after 10ms (constant 1.0): " << integ.value() << std::endl;

    return 0;
}
