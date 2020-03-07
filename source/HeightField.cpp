#include "heightfield/HeightField.h"

#include <unirender/Blackboard.h>
#include <unirender/RenderContext.h>

#include <algorithm>

#include <assert.h>

namespace hf
{

HeightField::HeightField(size_t width, size_t height)
    : ScalarField2D(width, height)
{
}

ur::TexturePtr HeightField::GetHeightmap()
{
    if (!m_heightmap || m_dirty) {
        UpdateGPU();
        m_dirty = false;
        assert(!m_cpu_dirty);
    }
    return m_heightmap;
}

const std::vector<float>& HeightField::GetValues() const
{
    if (m_cpu_dirty) {
        UpdateCPU();
        m_cpu_dirty = false;
    }
    return m_values;
}

float HeightField::Get(size_t x, size_t y) const
{
    if (m_cpu_dirty) {
        UpdateCPU();
        m_cpu_dirty = false;
    }
    const size_t idx = y * m_width + x;
    if (idx >= m_values.size()) {
        assert(0);
        return 0;
    } else {
        return m_values[idx];
    }
}

float HeightField::Get(float x, float y) const
{
    if (m_cpu_dirty) {
        UpdateCPU();
        m_cpu_dirty = false;
    }
    if (x < 0 || x >= m_width ||
        y < 0 || y >= m_height) {
        assert(0);
        return 0;
    }

    const size_t ix = static_cast<size_t>(std::floor(x));
    const size_t iy = static_cast<size_t>(std::floor(y));
    const size_t nix = (ix == m_width - 1) ? ix : ix + 1;
    const size_t niy = (iy == m_height - 1) ? iy : iy + 1;
    const float h00 = Get(ix, iy);
    const float h10 = Get(nix, iy);
    const float h01 = Get(ix, niy);
    const float h11 = Get(nix, niy);

    const float dx = x - ix;
    const float dy = y - iy;
    return (h00 * (1 - dx) + h10 * dx) * (1 - dy) + (h01 * (1 - dx) + h11 * dx) * dy;
}

float HeightField::Get(size_t idx) const
{
    if (m_cpu_dirty) {
        UpdateCPU();
        m_cpu_dirty = false;
    }
    if (idx >= m_values.size()) {
        assert(0);
        return 0;
    }
    else {
        return m_values[idx];
    }
}

void HeightField::SetShortValues(const std::vector<short>& values)
{
    if (m_width * m_height != values.size()) {
        return;
    }

    m_short_values = values;

    m_dirty = true;
}

void HeightField::UpdateCPU() const
{
    auto& rc = ur::Blackboard::Instance()->GetRenderContext();

    auto fbo = rc.CreateRenderTarget(0);
    assert(fbo != 0);

    int vp_x, vp_y, vp_w, vp_h;
    rc.GetViewport(vp_x, vp_y, vp_w, vp_h);

    rc.BindRenderTarget(fbo);
    rc.BindRenderTargetTex(m_heightmap->TexID(), ur::ATTACHMENT_COLOR0);
    rc.SetViewport(0, 0, m_width, m_height);
    assert(rc.CheckRenderTargetStatus());

    uint8_t* pixels = new uint8_t[m_width * m_height * 4];
    rc.ReadPixels(pixels, 4, 0, 0, m_width, m_height);

    rc.UnbindRenderTarget();
    rc.SetViewport(vp_x, vp_y, vp_w, vp_h);
    rc.ReleaseRenderTarget(fbo);

    std::vector<float> heights(m_width * m_height);
    for (size_t i = 0, n = heights.size(); i < n; ++i) {
        heights[i] = pixels[i * 4] / 255.0f;
    }
    delete[] pixels;

    SetValues(heights);
}

void HeightField::UpdateGPU() const
{
    if (m_width == 0 || m_height == 0) {
        return;
    }

    auto& rc = ur::Blackboard::Instance()->GetRenderContext();

    m_heightmap = std::make_shared<ur::Texture>();

    if (m_short_values.empty())
    {
        std::vector<short> pixels;
        pixels.resize(m_values.size());
        for (size_t i = 0, n = m_values.size(); i < n; ++i) {
            const auto f01 = std::min(std::max(m_values[i], 0.0f), 1.0f);
            pixels[i] = static_cast<short>(f01 * 0xffff);
        }

        m_heightmap->Upload(&rc, m_width, m_height, ur::TEXTURE_R16, pixels.data());
    }
    else
    {
        assert(m_short_values.size() == m_width * m_height);
        m_heightmap->Upload(&rc, m_width, m_height, ur::TEXTURE_R16, m_short_values.data());
    }
}

}