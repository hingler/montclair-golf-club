#ifndef BLUR_IMAGE_H_
#define BLUR_IMAGE_H_

#include "image/GenericImage.hpp"
#include "traits/Interpolatable.hpp"

#include "image/filter/GaussianBlur.hpp"

namespace image {
  namespace filter {
    template <typename DataType>
    bool BlurImage1D(const GenericImage<DataType>& input, GenericImage<DataType>& output, double sigma, bool isHorizontal) {
      static_assert(traits::interpolatable<DataType>::value);

      glm::ivec2 cursor;
      int direction = (isHorizontal ? 0 : 1);

      auto dims = input.Dimensions();
      auto dims_output = output.Dimensions();

      if (dims.width != dims_output.width || dims.height != dims_output.height) {
        // invalid for blur
        return false;
      }

      // prep gaussian kernel
      int size_max = static_cast<int>(ceil(4 * sigma));
      double* blur_cache = new double[2 * size_max + 1];
      double blur_sum = 0.0;
      for (int i = -size_max; i <= size_max; i++) {
        blur_cache[i + size_max] = GetGaussianIntensity(sigma, static_cast<double>(i));
        blur_sum += blur_cache[i + size_max];
      }

      blur_sum = 1.0 / blur_sum;

      for (int i = -size_max; i <= size_max; i++) {
        // i think that's unnecessary lol
        // just in case
        blur_cache[i + size_max] *= blur_sum;
      }
      

      for (int y = 0; y < dims.height; y++) {
        for (int x = 0; x < dims.width; x++) {
          cursor.x = x;
          cursor.y = y;
          
          cursor[direction] -= size_max;
          DataType output_cache;
          for (int b = -size_max; b <= size_max; b++) {
            // oob is a 0 i guess
            const DataType* sample = input.Get(cursor.x, cursor.y);
            if (sample != nullptr) {
              output_cache = output_cache + (*sample * blur_cache[b + size_max]);
              // data is normalized anyway so we're good!
            }
            cursor[direction]++;
          }

          output.Set(x, y, output_cache);
        }
      }

      delete[] blur_cache;

      return true;
    }
    
    /**
     * @brief Blurs a provided image, outputting the result to a provided output.
     * 
     * @tparam DataType 
     * @param input 
     * @param output 
     */
    template <typename DataType>
    bool BlurImage(const GenericImage<DataType>& input, GenericImage<DataType>& output, double sigma) {
      static_assert(traits::interpolatable<DataType>::value);

      auto dims = input.Dimensions();
      auto dims_output = output.Dimensions();

      if (dims.width != dims_output.width || dims.height != dims_output.height) {
        // invalid for blur
        return false;
      }

      GenericImage<DataType> blur1D(dims.width, dims.height);

      if (!BlurImage1D(input, blur1D, sigma, true)) {
        return false;
      }


      if (!BlurImage1D(blur1D, output, sigma, false)) {
        return false;
      }

      return true;
    }
  }
}

#endif // BLUR_IMAGE_H_