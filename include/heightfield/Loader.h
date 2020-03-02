#pragma once

#include <memory>
#include <string>

namespace hf
{

class HeightField;

class Loader
{
public:
    static std::shared_ptr<HeightField>
        Load(const std::string& filepath);

}; // Loader

}