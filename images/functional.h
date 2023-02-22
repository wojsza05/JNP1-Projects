#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include <cstddef>

template<typename T = std::nullptr_t>
auto compose() {
    return [](auto args) {
        return args;
    };
}

template <typename T>
auto compose(T func) {
    return [func](auto ... args) {
        return func(args...);
    };
}

template <typename T, typename... Funcs>
auto compose(T first_func, Funcs ... others) {
    return [first_func, others...](auto ... args) {
        return compose(others...)(first_func(args...));
    };
}

template<typename H>
auto lift(H h) {
    return [h](auto ... args) {
        return h();
    };
}

template <typename H, typename F>
auto lift(H h, F f) {
    return [h, f](auto ... args) {
        return h(f(args...));
    };
}

template <typename H, typename F, typename... Funcs>
auto lift(H h, F f, Funcs ... others) {
    return [h, f, others...](auto ... args) {
        return h(f(args...), others(args...)...);
    };
}

#endif //FUNCTIONAL_H
