#include "heightfield/HeightField.h"

#include <unirender/Texture.h>
#include <unirender/Device.h>
#include <unirender/Context.h>
#include <unirender/Framebuffer.h>
#include <unirender/Factory.h>
#include <unirender/TextureDescription.h>

#include <algorithm>

#include <assert.h>

namespace hf
{

HeightField::HeightField(size_t width, size_t height)
    : ScalarField2D(width, height)
{
}

ur::TexturePtr HeightField::GetHeightmap(const ur::Device& dev) const
{
    if (!m_heightmap || m_dirty) {
        UpdateGPU(dev);
        m_dirty = false;
        assert(!m_cpu_dirty);
    }
    return m_heightmap;
}

const std::vector<int32_t>& HeightField::GetValues(const ur::Device& dev) const
{
    if (m_cpu_dirty) {
        UpdateCPU(dev);
        m_cpu_dirty = false;
    }
    return m_values;
}

int32_t HeightField::Get(const ur::Device& dev, size_t x, size_t y) const
{
    if (m_cpu_dirty) {
        UpdateCPU(dev);
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

int32_t HeightField::Get(const ur::Device& dev, float x, float y) const
{
    if (m_cpu_dirty) {
        UpdateCPU(dev);
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
    const int32_t h00 = Get(dev, ix, iy);
    const int32_t h10 = Get(dev, nix, iy);
    const int32_t h01 = Get(dev, ix, niy);
    const int32_t h11 = Get(dev, nix, niy);

    const float dx = x - ix;
    const float dy = y - iy;
    return static_cast<int32_t>((h00 * (1 - dx) + h10 * dx) * (1 - dy) + (h01 * (1 - dx) + h11 * dx) * dy);
}

int32_t HeightField::Get(const ur::Device& dev, size_t idx) const
{
    if (m_cpu_dirty) {
        UpdateCPU(dev);
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

void HeightField::UpdateCPU(const ur::Device& dev) const
{
    auto ctx = ur::CreateContext(ur::APIType::OpenGL, dev);

    auto fbo = dev.CreateFramebuffer();
    //fbo->Bind();

    fbo->SetAttachment(ur::AttachmentType::Color0, ur::TextureTarget::Texture2D, m_heightmap, nullptr);

    //int vp_x, vp_y, vp_w, vp_h;
    //rc.GetViewport(vp_x, vp_y, vp_w, vp_h);

    ctx->SetViewport(0, 0, m_width, m_height);

    int16_t* pixels = new int16_t[m_width * m_height * 4];
    dev.ReadPixels(pixels, ur::TextureFormat::RGBA8, 0, 0, m_width, m_height);

    //rc.UnbindRenderTarget();
    //rc.SetViewport(vp_x, vp_y, vp_w, vp_h);
    //rc.ReleaseRenderTarget(fbo);

    std::vector<int32_t> heights(m_width * m_height);
    for (size_t i = 0, n = heights.size(); i < n; ++i) {
        heights[i] = pixels[i * 4];
    }
    delete[] pixels;

    SetValues(heights);
}

void HeightField::UpdateGPU(const ur::Device& dev) const
{
    if (m_width == 0 || m_height == 0) {
        return;
    }

    assert(m_values.size() == m_width * m_height);

    auto& src = m_values;
    std::vector<int16_t> dst(src.size());
    int32_t min = std::numeric_limits<short>::min();
    int32_t max = std::numeric_limits<short>::max();
    for (size_t i = 0, n = src.size(); i < n; ++i) {
        dst[i] = std::min(max, std::max(src[i], min));
    }

    ur::TextureDescription desc;
    desc.target = ur::TextureTarget::Texture2D;
    desc.width = m_width;
    desc.height = m_height;
    desc.format = ur::TextureFormat::R16;
    m_heightmap = dev.CreateTexture(desc, dst.data());
}

}