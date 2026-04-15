#pragma once

// Generic 2 component vector
template<typename T>
struct GenericVector {
    GenericVector(T x, T y) : x(x), y(y) {}
    T x;
    T y;
};
