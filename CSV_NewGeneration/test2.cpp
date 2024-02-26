#include <iostream>
#include <coroutine>
#include <vector>

template<typename T>
struct MyGenerator {
    struct promise_type {
        std::vector<T> values;

        auto get_return_object() {
            return MyGenerator{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }

        std::suspend_always initial_suspend() noexcept {
            return {};
        }

        std::suspend_always final_suspend() noexcept {
            return {};
        }

        void return_void() {}

        std::suspend_always yield_value(T value) noexcept {
            values.push_back(value);
            return {};
        }

        void unhandled_exception() {
            std::terminate();
        }
    };

    std::coroutine_handle<promise_type> coroutine;

    ~MyGenerator() {
        if (coroutine)
            coroutine.destroy();
    }

    bool move_next() {
        coroutine.resume();
        return !coroutine.done();
    }

    T current_value() const {
        return coroutine.promise().values.back();
    }
};

MyGenerator<int> generateInts() {
    co_yield 1;
    co_yield 2;
    co_yield 3;
}

int main() {
    MyGenerator<int> gen = generateInts();
    while (gen.move_next()) {
        std::cout << gen.current_value() << std::endl;
    }
    return 0;
}
