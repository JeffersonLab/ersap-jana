#ifndef ERSAP_DEMO_DATA_IMAGE_HPP_
#define ERSAP_DEMO_DATA_IMAGE_HPP_

#include <image.hpp>

#include <ersap/engine_data_type.hpp>

namespace ersap {
namespace jana {

using byte_t = std::uint8_t;
using bytes_t = std::vector<byte_t>;

class ByteBuffer
{

public:
    ByteBuffer(size_t size)
    {
        data_.reserve(size);
    }

public:
    void put(std::int32_t v)
    {
        data_.insert(data_.end(), (byte_t*) &v, (byte_t*) &v + sizeof(int));
    }

    template<typename T>
    void putRange(const T& v)
    {
        data_.insert(data_.end(), v.begin(), v.end());
    }

public:
    bytes_t data_;
};


const extern ersap::EngineDataType IMAGE_TYPE;

} // end namespace jana

} // end namespace ersap

#endif
