#pragma once

#include "heightfield/ScalarField2D.h"

#include <unirender/Texture.h>

#include <vector>

namespace hf
{

class HeightField : public ScalarField2D<int32_t>
{
public:
    HeightField(size_t width, size_t height);

    ur::TexturePtr GetHeightmap() const;

    const std::vector<int32_t>& GetValues() const;

    int32_t Get(size_t x, size_t y) const;
    int32_t Get(float x, float y) const;
    int32_t Get(size_t idx) const;

    void SetCPUDirty() { m_cpu_dirty = true; }

private:
    void UpdateCPU() const;
    void UpdateGPU() const;

private:
    mutable ur::TexturePtr m_heightmap = nullptr;

    mutable bool m_cpu_dirty = false;

}; // HeightField

}