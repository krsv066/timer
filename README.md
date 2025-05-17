# C++ Execution Timer

A lightweight, header-only C++ library for measuring execution time of functions, methods, functors, and lambdas.

## Features

- Measures execution time of any callable entity
- Supports:
  - Class methods (both const and non-const)
  - Free functions
  - Functors
  - Lambda expressions

## Requirements

- C++20 compatible compiler
- CMake 3.28 or higher (for building the examples and tests)

## Installation

This is a header-only library, so you can simply include the `timer.h` file in your project.

## Usage Examples

### Timing Class Methods

```cpp
#include "timer.h"

class MyClass {
public:
    int Calculate(int x) {
        // Your calculation logic here
        return x * 2;
    }
    
    void Process() const {
        // Your processing logic here
    }
};

int main() {
    MyClass obj;
    
    // Time a non-const method and get the result
    auto result = timer::ExecutionTimer(&obj)->Calculate(5);
    
    // Time a const method
    timer::ExecutionTimer(&obj)->Process();
    
    return 0;
}
```

### Timing Functions

```cpp
#include "timer.h"
#include <functional>

int Multiply(int x) {
    // Your function logic
    return x * 3;
}

void Process() {
    // Your processing logic
}

int main() {
    // Time a function with return value
    auto result = timer::ExecutionTimer(Multiply)(7);
    
    // Time a void function
    (timer::ExecutionTimer(Process))();
    
    return 0;
}
```

### Timing Functors

```cpp
#include "timer.h"
#include <functional>

// A simple functor
class Multiplier {
public:
    Multiplier(int factor) : factor_(factor) {}
    
    int operator()(int x) const {
        // Functor logic
        return x * factor_;
    }
    
private:
    int factor_;
};

int main() {
    // Create a functor
    Multiplier mult(5);
    
    // Time the functor execution
    auto result = timer::ExecutionTimer<int(int)>(mult)(10);
    std::cout << "Result: " << result << std::endl;  // Output: Result: 50
    
    return 0;
}
```

### Timing Lambda Expressions

```cpp
#include "timer.h"
#include <functional>

int main() {
    // Time a lambda function
    auto lambda = [](int x) {
        // Lambda logic
        return x * 4;
    };
    
    auto result = timer::ExecutionTimer<int(int)>(lambda)(10);
    
    return 0;
}
```

## Building and Testing

This project uses CMake for building and Google Test for testing.

```bash
# Clone the repository
git clone https://github.com/yourusername/timer.git
cd timer

# Create a build directory
mkdir build && cd build

# Configure and build
cmake ..
cmake --build .

# Run the tests
ctest
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.
