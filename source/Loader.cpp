#include "heightfield/Loader.h"
#include "heightfield/HeightField.h"

#include <gimg_import.h>
#include <gimg_typedef.h>
#include <gis/ElevationLoader.h>

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

            std::vector<float> height(sz, 0.0f);
            for (size_t i = 0; i < sz; ++i) {
                height[i] = static_cast<unsigned char>(pixels[i]) / 255.0f;
            }

            hf = std::make_shared<hf::HeightField>(size, size);
            hf->SetValues(height);
        }
        fin.close();
    }
    else if (ext == ".hgt")
    {
        size_t width, height;
        std::vector<int16_t> vals;
        if (gis::ElevationLoader::Load(filepath, width, height, vals))
        {
            assert(width * height == vals.size());

            hf = std::make_shared<hf::HeightField>(width, height);
            hf->SetShortValues(vals);
        }
    }
    else
    {
	    int width, height, format;
	    uint8_t* pixels = gimg_import(filepath.c_str(), &width, &height, &format);
        if (!pixels) {
            return nullptr;
        }

        size_t sz = width * height;
        std::vector<float> h_data(sz, 0.0f);
        switch (format)
        {
        case GPF_ALPHA:
        case GPF_LUMINANCE:
        case GPF_LUMINANCE_ALPHA:
            for (size_t i = 0; i < sz; ++i) {
                h_data[i] = pixels[i] / 255.0f;
            }
            break;
        case GPF_RGB:
            for (size_t i = 0; i < sz; ++i) {
                h_data[i] = pixels[i * 3] / 255.0f;
            }
            break;
        case GPF_RGBA8:
            for (size_t i = 0; i < sz; ++i) {
                h_data[i] = pixels[i * 4] / 255.0f;
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