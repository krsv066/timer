#include <gtest/gtest.h>
#include "../timer.h"
#include <functional>
#include <sstream>
#include <thread>

// Helper class to test method timing
class TestClass {
public:
    int TestMethod(int x) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        return x * 2;
    }

    void VoidMethod() const {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
};

// Helper functions to test function timing
int TestFunction(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(40));
    return x * 3;
}

void VoidFunction() {
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
}

// Functor for testing
class TestFunctor {
public:
    TestFunctor(int factor) : factor_(factor) {
    }

    int operator()(int x) const {
        std::this_thread::sleep_for(std::chrono::milliseconds(35));
        return x * factor_;
    }

private:
    int factor_;
};

// Test fixture
class TimerTest : public ::testing::Test {
protected:
    // Redirect cout to capture output
    void SetUp() override {
        originalBuffer = std::cout.rdbuf();
        std::cout.rdbuf(captureStream.rdbuf());
    }

    void TearDown() override {
        std::cout.rdbuf(originalBuffer);
    }

    std::stringstream captureStream;
    std::streambuf* originalBuffer;

    // Helper to verify that the output contains timing information
    void VerifyTimingOutput() {
        std::string output = captureStream.str();
        captureStream.str("");  // Clear the stream for next test

        EXPECT_TRUE(output.find("Time: ") != std::string::npos);

        // Extract the timing value
        size_t pos = output.find("Time: ");
        std::string time_str = output.substr(pos + 6);

        // Check that it contains a numeric value and "s" for seconds
        EXPECT_TRUE(time_str.find("s") != std::string::npos);
    }
};

// Test class method timing
TEST_F(TimerTest, ClassMethodTiming) {
    TestClass test_obj;

    // Test non-const method
    auto result = timer::ExecutionTimer(&test_obj)->TestMethod(5);

    // Verify the result is correct
    EXPECT_EQ(result, 10);

    // Verify timing output
    VerifyTimingOutput();
}

// Test const method timing
TEST_F(TimerTest, ConstMethodTiming) {
    const TestClass test_obj;

    timer::ExecutionTimer(&test_obj)->VoidMethod();

    // Verify timing output
    VerifyTimingOutput();
}

// Test function timing
TEST_F(TimerTest, FunctionTiming) {
    auto result = timer::ExecutionTimer(TestFunction)(7);

    // Verify the result is correct
    EXPECT_EQ(result, 21);

    // Verify timing output
    VerifyTimingOutput();
}

// Test void function timing
TEST_F(TimerTest, VoidFunctionTiming) {
    (timer::ExecutionTimer(VoidFunction))();

    // Verify timing output
    VerifyTimingOutput();
}

// Test functor timing
TEST_F(TimerTest, FunctorTiming) {
    // Create a functor that multiplies by 5
    TestFunctor multiplier(5);

    // Time the functor execution
    auto result = timer::ExecutionTimer<int(int)>(multiplier)(10);

    // Verify the result is correct
    EXPECT_EQ(result, 50);

    // Verify timing output
    VerifyTimingOutput();
}

// Test lambda timing
TEST_F(TimerTest, LambdaTiming) {
    auto lambda = [](int x) {
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
        return x * 4;
    };

    auto result = timer::ExecutionTimer<int(int)>(lambda)(10);

    // Verify the result is correct
    EXPECT_EQ(result, 40);

    // Verify timing output
    VerifyTimingOutput();
}

// Test proxy const-correctness
TEST_F(TimerTest, ProxyConstCorrectness) {
    TestClass test_obj;
    const TestClass const_test_obj;

    // Non-const object, non-const method
    timer::ExecutionTimer(&test_obj)->TestMethod(5);
    VerifyTimingOutput();

    // Const object, const method
    timer::ExecutionTimer(&const_test_obj)->VoidMethod();
    VerifyTimingOutput();
}
