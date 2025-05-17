#include <chrono>
#include <cstddef>
#include <iostream>
#include <type_traits>

namespace timer {

namespace detail {

template <typename T>
class TimingProxy {
public:
    TimingProxy(T* obj) : obj_{obj} {
        start_ = std::chrono::high_resolution_clock::now();
    }

    ~TimingProxy() {
        end_ = std::chrono::high_resolution_clock::now();
        const std::chrono::duration<double> diff = end_ - start_;
        std::cout << "Time: " << diff << std::endl;
    }

    const T* operator->() const {
        return obj_;
    }

    T* operator->() {
        return obj_;
    }

private:
    T* obj_;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
    std::chrono::time_point<std::chrono::high_resolution_clock> end_;
};

}  // namespace detail

template <typename T>
class ExecutionTimer {
public:
    ExecutionTimer() : obj_{nullptr} {
    }

    ExecutionTimer(T* obj) : obj_{obj} {
    }

    const detail::TimingProxy<T> operator->() const {
        return detail::TimingProxy(obj_);
    }

    detail::TimingProxy<T> operator->() {
        return detail::TimingProxy(obj_);
    }

private:
    T* obj_;
};

template <typename R, typename... Args>
class ExecutionTimer<R(Args...)> {
public:
    ExecutionTimer(std::function<R(Args...)> func) : func_(func) {
    }

    auto operator()(Args... args) {
        auto start = std::chrono::high_resolution_clock::now();

        if constexpr (std::is_void_v<R>) {
            func_(std::forward<Args>(args)...);
            auto end = std::chrono::high_resolution_clock::now();
            const std::chrono::duration<double> diff = end - start;
            std::cout << "Time: " << diff << std::endl;
        } else {
            R result = func_(std::forward<Args>(args)...);
            auto end = std::chrono::high_resolution_clock::now();
            const std::chrono::duration<double> diff = end - start;
            std::cout << "Time: " << diff << std::endl;
            return result;
        }
    }

private:
    std::function<R(Args...)> func_;
};

}  // namespace timer
