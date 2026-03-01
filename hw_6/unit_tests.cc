// HW6 unit tests: Stopwatch, Filter, Integrator
#include <cmath>
#include <chrono>
#include <thread>

#include "gtest/gtest.h"
#include "stopwatch.h"
#include "elma.h"
#include "random_process.h"
#include "filter.h"
#include "integrator.h"

using namespace std::chrono;
using namespace elma;

#define SLEEP_MS(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))

// Tolerance for timing tests (seconds)
const double TIME_TOL = 0.05;

// Process that sends 0.25 and 0.75 alternately to "link" (for Filter test)
class AlternatingProcess : public Process {
public:
    AlternatingProcess(std::string name) : Process(name), toggle_(false) {}
    void init() override {}
    void start() override {}
    void update() override {
        double v = toggle_ ? 0.75 : 0.25;
        toggle_ = !toggle_;
        channel("link").send(nlohmann::json(v));
    }
    void stop() override {}

private:
    bool toggle_;
};

// Process that sends a constant value every update (for Integrator test)
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

// --- Stopwatch tests ---
TEST(Stopwatch, GetSecondsAfterSleep) {
    Stopwatch w;
    w.start();
    SLEEP_MS(1500);
    w.stop();
    double s = w.get_seconds();
    EXPECT_GE(s, 1.4);
    EXPECT_LE(s, 1.6);
}

TEST(Stopwatch, GetSecondsUnchangedWhenStopped) {
    Stopwatch w;
    w.start();
    SLEEP_MS(500);
    w.stop();
    double s1 = w.get_seconds();
    SLEEP_MS(500);
    double s2 = w.get_seconds();
    EXPECT_NEAR(s1, s2, TIME_TOL);
}

TEST(Stopwatch, ResetZeros) {
    Stopwatch w;
    w.start();
    SLEEP_MS(200);
    w.stop();
    w.reset();
    EXPECT_NEAR(w.get_seconds(), 0.0, 0.001);
    EXPECT_NEAR(w.get_milliseconds(), 0.0, 0.001);
    EXPECT_NEAR(w.get_nanoseconds(), 0.0, 1e6);
}

TEST(Stopwatch, GetMinutesMillisecondsNanoseconds) {
    Stopwatch w;
    w.start();
    SLEEP_MS(1500);  // 1.5 s
    w.stop();
    EXPECT_NEAR(w.get_seconds(), 1.5, TIME_TOL);
    EXPECT_NEAR(w.get_milliseconds(), 1500.0, 50.0);
    EXPECT_NEAR(w.get_minutes(), 1.5 / 60.0, TIME_TOL / 60.0);
    EXPECT_GE(w.get_nanoseconds(), 1e9 * 1.4);
    EXPECT_LE(w.get_nanoseconds(), 1e9 * 1.6);
}

// --- Filter tests ---
TEST(Filter, AlternatingProcessYieldsHalf) {
    Manager m;
    AlternatingProcess alt("alternating");
    Filter f("filter");
    Channel link("link");

    m.schedule(alt, 1_ms)
        .schedule(f, 1_ms)
        .add_channel(link)
        .init()
        .run(100_ms);

    // After many steps, running average of 0.25, 0.75, 0.25, ... should be 0.5
    EXPECT_NEAR(f.value(), 0.5, 0.05);
}

// --- Integrator tests ---
TEST(Integrator, ConstantInputRamp) {
    Manager m;
    ConstantProcess src("source", 1.0);
    Integrator integ("integrator");
    Channel link("link");

    m.schedule(src, 1_ms)
        .schedule(integ, 1_ms)
        .add_channel(link)
        .init()
        .run(10_ms);

    // integral += delta() * v each step; delta() is in milliseconds. 1ms period, ~10 steps, v=1 -> integral ≈ 10
    double val = integ.value();
    EXPECT_GE(val, 8.0);
    EXPECT_LE(val, 12.0);
}

TEST(Integrator, InitialValueZero) {
    Integrator integ("integrator");
    EXPECT_DOUBLE_EQ(integ.value(), 0.0);
}

TEST(Filter, InitialValueZero) {
    Filter f("filter");
    EXPECT_DOUBLE_EQ(f.value(), 0.0);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
