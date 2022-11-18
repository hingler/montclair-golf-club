#ifndef GENERIC_IMAGE_H_
#define GENERIC_IMAGE_H_

#include <cinttypes>
#include <cstring>

#include "image/image_dims.hpp"

namespace image {
  template <typename DataType>
  class GenericImage {
  public:

    GenericImage(uint32_t width, uint32_t height);

    GenericImage(const GenericImage<DataType>& other);
    GenericImage(GenericImage<DataType>&& other);

    GenericImage<DataType>& operator=(const GenericImage<DataType>& other);
    GenericImage<DataType>& operator=(GenericImage<DataType>&& other);

    virtual ~GenericImage();

    /**
     * @brief returns a non writable pointer to a terrain record, if it exists
     * 
     * @param x - x coordinate
     * @param y - y coordinate 
     * @return const DataType* 
     */
    const DataType* Get(uint32_t x, uint32_t y) const;

    /**
     * @brief assigns data to a given x/y coordinate, if in bounds
     * 
     * @param x - x coordinate
     * @param y - y coordinate
     * @param data - data to be written
     */
    void Set(uint32_t x, uint32_t y, const DataType& input);

    /**
     * @return dimensions of the image 
     */
    image_dims Dimensions() const;

    /**
     * @return underlying data pointer
     */
    const DataType* Data() const;
  private:
    image_dims dims_;
    DataType* data;
  };
}


// IMPLEMENTATION

namespace image {

  template <typename DataType>
  GenericImage<DataType>::GenericImage(uint32_t width, uint32_t height) : dims_(width, height) {
    data = new DataType[width * height];
  }

  template <typename DataType>
  GenericImage<DataType>::GenericImage(const GenericImage<DataType>& other) : dims_(other.dims_) {
    data = new DataType[dims_.width * dims_.height];
    memcpy(data, other.data, dims_.width * dims_.height * sizeof(DataType));
  }

  template <typename DataType>
  GenericImage<DataType>::GenericImage(GenericImage<DataType>&& other) : dims_(other.dims_), data(other.data) {
    other.data = nullptr;
  }

  template <typename DataType>
  GenericImage<DataType>& GenericImage<DataType>::operator=(const GenericImage<DataType>& other) {
    dims_ = other.dims_;
    if (data != nullptr) {
      delete[] data;
    }

    data = new DataType[dims_.width * dims_.height];
    memcpy(data, other.data, dims_.width * dims_.height * sizeof(DataType));
  }

  template <typename DataType>
  GenericImage<DataType>& GenericImage<DataType>::operator=(GenericImage<DataType>&& other) {
    dims_ = other.dims_;
    data = other.data;
    other.data = nullptr;
  }

  template <typename DataType>
  GenericImage<DataType>::~GenericImage() {
    if (data != nullptr) {
      delete[] data;
    }
  }

  template <typename DataType>
  const DataType* GenericImage<DataType>::Get(uint32_t x, uint32_t y) const {
    if (x < dims_.width && y < dims_.height) {
      return const_cast<const DataType*>(data + (y * dims_.width + x));
    }

    return nullptr;
  }

  template <typename DataType>
  void GenericImage<DataType>::Set(uint32_t x, uint32_t y, const DataType& input) {
    if (x < dims_.width && y < dims_.height) {
      memcpy((data + (y * dims_.width + x)), &input, sizeof(DataType));
    }
  }

  template <typename DataType>
  image_dims GenericImage<DataType>::Dimensions() const {
    return dims_;
  }

  template <typename DataType>
  const DataType* GenericImage<DataType>::Data() const {
    return data;
  }
}

// generic image can hold terrain data as well as rgba data (uint8_t fits perfectly!!)

#endif // GENERIC_IMAGE_H_