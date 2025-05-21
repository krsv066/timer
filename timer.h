#include <chrono>
#include <functional>
#include <iostream>
#include <type_traits>

namespace timer {

namespace detail {

class TimerBase {
public:
    TimerBase() : start{std::chrono::high_resolution_clock::now()} {
    }

    ~TimerBase() {
        end = std::chrono::high_resolution_clock::now();
        const std::chrono::duration<double> diff = end - start;
        std::cout << "Time: " << diff << std::endl;
    }

protected:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> end;
};

template <typename T>
class TimerProxy final : private TimerBase {
public:
    TimerProxy(T* obj) : obj_{obj} {
    }

    ~TimerProxy() = default;

    const T* operator->() const {
        return obj_;
    }

    T* operator->() {
        return obj_;
    }

private:
    T* obj_;
};

}  // namespace detail

template <typename T>
class ExecutionTimer {
public:
    ExecutionTimer(T* obj) : obj_{obj} {
    }

    const detail::TimerProxy<T> operator->() const {
        return detail::TimerProxy(obj_);
    }

    detail::TimerProxy<T> operator->() {
        return detail::TimerProxy(obj_);
    }

private:
    T* obj_;
};

template <typename R, typename... Args>
class ExecutionTimer<R(Args...)> {
public:
    ExecutionTimer(std::function<R(Args...)> func) : func_{func} {
    }

    auto operator()(Args... args) {
        detail::TimerBase timer;
        if constexpr (std::is_void_v<R>) {
            func_(std::forward<Args>(args)...);
        } else {
            R result = func_(std::forward<Args>(args)...);
            return result;
        }
    }

private:
    std::function<R(Args...)> func_;
};

}  // namespace timer
