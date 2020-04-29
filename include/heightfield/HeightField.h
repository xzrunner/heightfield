#pragma once

#include "heightfield/ScalarField2D.h"

#include <unirender/typedef.h>

#include <vector>

namespace ur { class Device; }

namespace hf
{

class HeightField : public ScalarField2D<int32_t>
{
public:
    HeightField(size_t width, size_t height);

    ur::TexturePtr GetHeightmap(const ur::Device& dev) const;

    const std::vector<int32_t>& GetValues(const ur::Device& dev) const;

    int32_t Get(const ur::Device& dev, size_t x, size_t y) const;
    int32_t Get(const ur::Device& dev, float x, float y) const;
    int32_t Get(const ur::Device& dev, size_t idx) const;

    void SetCPUDirty() { m_cpu_dirty = true; }

private:
    void UpdateCPU(const ur::Device& dev) const;
    void UpdateGPU(const ur::Device& dev) const;

private:
    mutable ur::TexturePtr m_heightmap = nullptr;

    mutable bool m_cpu_dirty = false;

}; // HeightField

}