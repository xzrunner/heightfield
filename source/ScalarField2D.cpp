#include "heightfield/ScalarField2D.h"

#include <assert.h>

namespace hf
{

ScalarField2D::ScalarField2D(size_t width, size_t height)
    : m_width(width)
    , m_height(height)
{
    m_values.resize(m_width * m_height, 0);
}

ScalarField2D::ScalarField2D(size_t width, size_t height, float val)
    : m_width(width)
    , m_height(height)
{
    m_values.resize(m_width * m_height, val);
}

void ScalarField2D::SetValues(const std::vector<float>& values) const
{
    if (m_width * m_height != values.size()) {
        return;
    }

    assert(m_values.size() == values.size());
    m_values = values;

    m_dirty = true;
}

bool ScalarField2D::Set(size_t x, size_t y, float h)
{
    const size_t idx = y * m_width + x;
    if (idx >= m_values.size()) {
        assert(0);
        return false;
    } else {
        m_values[idx] = h;
        m_dirty = true;
        return true;
    }
}

float ScalarField2D::Get(size_t x, size_t y) const
{
    const size_t idx = y * m_width + x;
    if (idx >= m_values.size()) {
        assert(0);
        return 0;
    } else {
        return m_values[idx];
    }
}

bool ScalarField2D::Add(size_t x, size_t y, float dh)
{
    if (x < 0 || x >= m_width ||
        y < 0 || y >= m_height) {
        return false;
    }

    const size_t idx = y * m_width + x;
    m_values[idx] += dh;
    m_dirty = true;

    return true;
}

bool ScalarField2D::Add(size_t idx, float dh)
{
    if (idx >= m_values.size()) {
        assert(0);
        return false;
    }

    m_values[idx] += dh;
    m_dirty = true;

    return true;
}

bool ScalarField2D::Inside(int x, int y) const
{
    if (x < 0 || x >= static_cast<int>(m_width) ||
        y < 0 || y >= static_cast<int>(m_height)) {
        return false;
    } else {
        return true;
    }
}

void ScalarField2D::Scale(float scale)
{
    for (auto& v : m_values) {
        v *= scale;
    }
    m_dirty = true;
}

void ScalarField2D::Normalize()
{
    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();
    for (auto& v : m_values)
    {
        if (v < min) {
            min = v;
        }
        if (v > max) {
            max = v;
        }
    }
    if (min == max) {
        return;
    }

    for (auto& v : m_values) {
        v = (v - min) / (max - min);
    }

    m_dirty = true;
}

}