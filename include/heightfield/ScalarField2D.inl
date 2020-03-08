#pragma once

#include <assert.h>

namespace hf
{

template <typename T>
ScalarField2D<T>::ScalarField2D(size_t width, size_t height)
    : m_width(width)
    , m_height(height)
{
    m_values.resize(m_width * m_height, 0);
}

template <typename T>
ScalarField2D<T>::ScalarField2D(size_t width, size_t height, T val)
    : m_width(width)
    , m_height(height)
{
    m_values.resize(m_width * m_height, val);
}

template <typename T>
void ScalarField2D<T>::SetValues(const std::vector<T>& values) const
{
    if (m_width * m_height != values.size()) {
        return;
    }

    assert(m_values.size() == values.size());
    m_values = values;

    m_dirty = true;
}

template <typename T>
bool ScalarField2D<T>::Set(size_t x, size_t y, T h)
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

template <typename T>
T ScalarField2D<T>::Get(size_t x, size_t y) const
{
    const size_t idx = y * m_width + x;
    if (idx >= m_values.size()) {
        assert(0);
        return 0;
    } else {
        return m_values[idx];
    }
}

template <typename T>
bool ScalarField2D<T>::Add(size_t x, size_t y, T dh)
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

template <typename T>
bool ScalarField2D<T>::Add(size_t idx, T dh)
{
    if (idx >= m_values.size()) {
        assert(0);
        return false;
    }

    m_values[idx] += dh;
    m_dirty = true;

    return true;
}

template <typename T>
bool ScalarField2D<T>::Inside(int x, int y) const
{
    if (x < 0 || x >= static_cast<int>(m_width) ||
        y < 0 || y >= static_cast<int>(m_height)) {
        return false;
    } else {
        return true;
    }
}

template <typename T>
void ScalarField2D<T>::Scale(float scale)
{
    for (auto& v : m_values) {
        v *= scale;
    }
    m_dirty = true;
}

}