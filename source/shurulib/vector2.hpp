#pragma once

// Generic 2 component vector
template<typename T>
struct GenericVector {
    GenericVector() : x(0), y(0) {}
    GenericVector(T x, T y) : x(x), y(y) {}
    T x;
    T y;
};
