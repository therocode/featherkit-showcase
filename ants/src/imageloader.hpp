#pragma once
#include <vector>
#include <string>

class ImageLoader
{
    public:
        std::vector<unsigned char> loadImage(std::string path);
};
