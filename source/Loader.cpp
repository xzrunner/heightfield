#include "heightfield/Loader.h"
#include "heightfield/HeightField.h"
#include "heightfield/Utility.h"

#include <gimg_import.h>
#include <gimg_typedef.h>

#include <boost/filesystem.hpp>

namespace hf
{

std::shared_ptr<hf::HeightField>
Loader::Load(const std::string& filepath)
{
    std::shared_ptr<hf::HeightField> hf = nullptr;

    if (filepath.empty()) {
        return nullptr;
    }

    auto ext = boost::filesystem::extension(filepath);
    std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
    if (ext == ".raw")
    {
        std::ifstream fin(filepath, std::ios::binary | std::ios::ate);
        if (fin.fail()) {
            return nullptr;
        }

        size_t sz = static_cast<size_t>(fin.tellg());
        fin.seekg(0, std::ios::beg);

        std::vector<char> pixels(sz, 0);
        if (fin.read(pixels.data(), sz))
        {
            size_t size = static_cast<size_t>(std::sqrt(sz));
            assert(size * size == sz);

            std::vector<int32_t> height(sz, 0);
            for (size_t i = 0; i < sz; ++i) {
                height[i] = Utility::HeightCharToShort(pixels[i]);
            }

            hf = std::make_shared<hf::HeightField>(size, size);
            hf->SetValues(height);
        }
        fin.close();
    }
    else
    {
	    int width, height, format;
	    uint8_t* pixels = gimg_import(filepath.c_str(), &width, &height, &format);
        if (!pixels) {
            return nullptr;
        }

        size_t sz = width * height;
        std::vector<int32_t> h_data(sz, 0);
        switch (format)
        {
        case GPF_ALPHA:
        case GPF_LUMINANCE:
        case GPF_LUMINANCE_ALPHA:
            for (size_t i = 0; i < sz; ++i) {
                h_data[i] = Utility::HeightUCharToShort(pixels[i]);
            }
            break;
        case GPF_RGB:
            for (size_t i = 0; i < sz; ++i) {
                h_data[i] = Utility::HeightUCharToShort(pixels[i * 3]);
            }
            break;
        case GPF_RGBA8:
            for (size_t i = 0; i < sz; ++i) {
                h_data[i] = Utility::HeightUCharToShort(pixels[i * 4]);
            }
            break;
        case GPF_R16:
        {
            uint16_t* pixels16 = (uint16_t*)pixels;
            for (size_t i = 0; i < sz; ++i) {
                h_data[i] = pixels16[i];
            }
        }
            break;
        default:
            assert(0);
        }

        hf = std::make_shared<hf::HeightField>(width, height);
        hf->SetValues(h_data);
    }

    return hf;
}

}