#pragma once

#include <vector>

namespace hf
{

template <typename T>
struct ScalarValue
{
    ScalarValue() { }
    ScalarValue(int a, int b, T h) : x(a), y(b), value(h) { }

    size_t x, y;
	T value;
};

template <typename T>
class ScalarField2D
{
public:
    ScalarField2D(size_t width, size_t height);
    ScalarField2D(size_t width, size_t height, T val);

    size_t Width() const { return m_width; }
    size_t Height() const { return m_height; }

    void SetValues(const std::vector<T>& values) const;

    bool Set(size_t x, size_t y, T h);
    T Get(size_t x, size_t y) const;

    bool Add(size_t x, size_t y, T dh);
    bool Add(size_t idx, T dh);

    bool Inside(int x, int y) const;

    void Scale(float scale);

protected:
    size_t m_width = 0, m_height = 0;

    mutable std::vector<T> m_values;

    mutable bool m_dirty = true;

}; // ScalarField2D

}

#include "heightfield/ScalarField2D.inl"