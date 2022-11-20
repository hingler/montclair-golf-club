// prepares necessary STB implementation

// temp, for intellisense
#include "noise/simplex_noise.hpp"
#include "course/sampler/InterpolatingImageSampler.hpp"
#include "course/terrain_data.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"