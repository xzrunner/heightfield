#pragma once

#include "heightfield/ScalarField2D.h"

#include <unirender2/typedef.h>

#include <vector>

namespace ur2 { class Device; }

namespace hf
{

class HeightField : public ScalarField2D<int32_t>
{
public:
    HeightField(size_t width, size_t height);

    ur2::TexturePtr GetHeightmap(const ur2::Device& dev) const;

    const std::vector<int32_t>& GetValues(const ur2::Device& dev) const;

    int32_t Get(const ur2::Device& dev, size_t x, size_t y) const;
    int32_t Get(const ur2::Device& dev, float x, float y) const;
    int32_t Get(const ur2::Device& dev, size_t idx) const;

    void SetCPUDirty() { m_cpu_dirty = true; }

private:
    void UpdateCPU(const ur2::Device& dev) const;
    void UpdateGPU(const ur2::Device& dev) const;

private:
    mutable ur2::TexturePtr m_heightmap = nullptr;

    mutable bool m_cpu_dirty = false;

}; // HeightField

}