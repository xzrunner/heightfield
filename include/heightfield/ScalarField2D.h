#pragma once

#include <vector>

namespace hf
{

struct ScalarValue
{
    ScalarValue() { }
    ScalarValue(int a, int b, float h) : x(a), y(b), value(h) { }

    size_t x, y;
	float value;
};

class ScalarField2D
{
public:
    ScalarField2D(size_t width, size_t height);
    ScalarField2D(size_t width, size_t height, float val);

    size_t Width() const { return m_width; }
    size_t Height() const { return m_height; }

    void SetValues(const std::vector<float>& values) const;

    bool Set(size_t x, size_t y, float h);
    float Get(size_t x, size_t y) const;

    bool Add(size_t x, size_t y, float dh);
    bool Add(size_t idx, float dh);

    bool Inside(int x, int y) const;

    void Scale(float scale);

    void Normalize();

protected:
    size_t m_width = 0, m_height = 0;

    mutable std::vector<float> m_values;

    mutable bool m_dirty = true;

}; // ScalarField2D

}