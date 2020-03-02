#pragma once

#include "heightfield/ScalarField2D.h"

#include <unirender/Texture.h>

#include <vector>

namespace hf
{

class HeightField : public ScalarField2D
{
public:
    HeightField(size_t width, size_t height);

    ur::TexturePtr GetHeightmap();

    const std::vector<float>& GetValues() const;

    float Get(size_t x, size_t y) const;
    float Get(float x, float y) const;
    float Get(size_t idx) const;

    void SetCPUDirty() { m_cpu_dirty = true; }

private:
    void UpdateCPU() const;
    void UpdateGPU() const;

private:
    mutable ur::TexturePtr m_heightmap = nullptr;

    mutable bool m_cpu_dirty = false;

}; // HeightField

}