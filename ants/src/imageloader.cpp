#include "imageloader.h"
#include "lodepng.h"
#include <iostream>

std::vector<unsigned char> ImageLoader::loadImage(std::string path)
{
    std::vector<unsigned char> image; //the raw pixels

    uint32_t width, height;

    //decode
    unsigned error = lodepng::decode(image, width, height, path);

    //if there's an error, display it
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

    return image;
}
