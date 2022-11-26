// prepares necessary STB implementation

// temp, for intellisense
#include "course/sampler/TurbulentDisplaceSampler.hpp"
#include "course/generator/ICourseGenerator.hpp"
#include "course/generator/BruteForceCourseGenerator.hpp"
#include "image/filter/BlurImage.hpp"

#include "course/path/ParameterizedCurve.hpp"
#include "course/path/BezierCurve.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"