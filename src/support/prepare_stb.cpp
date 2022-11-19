// prepares necessary STB implementation

// temp, for intellisense
#include "image/GenericImage.hpp"
#include "image/ColorUtils.hpp"
#include "image/ImageWriter.hpp"
#include "image/TerrainImageConverter.hpp"

#include "course/sampler/MetaballSampler.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"