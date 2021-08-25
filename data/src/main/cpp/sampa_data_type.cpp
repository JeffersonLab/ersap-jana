//
// Created by Vardan Gurjyan on 8/25/21.
//
#include "image_data_type.hpp"

#include <cstdint>
#include <iostream>

namespace ersap {
    namespace jana {

        const int IMG_META_SIZE = 5 * sizeof(std::int32_t);

        class ImageSerializer : public ersap::Serializer
        {
        public:
            std::vector<std::uint8_t> write(const ersap::any& data) const override
            {
                const auto& img = ersap::any_cast<const Image&>(data);

                auto name_size = img.name.size();
                auto img_size = img.mat.total() * img.mat.channels();
                auto total_bytes = IMG_META_SIZE + name_size + img_size;

                if (total_bytes > INT32_MAX) {
                    throw std::runtime_error{"image is too large"};
                }

                auto img_ptr = img.mat.ptr();
                auto img_data = std::vector<std::uint8_t>(img_ptr, img_ptr + img_size);

                auto buffer = ByteBuffer(total_bytes);
                buffer.put(name_size);
                buffer.put(img.mat.rows);
                buffer.put(img.mat.cols);
                buffer.put(img.mat.type());
                buffer.put(img_size);

                buffer.putRange(img.name);
                buffer.putRange(img_data);

                return buffer.data_;
            }

            ersap::any read(const std::vector<std::uint8_t>& buffer) const override
            {
                auto name_size= to_int(buffer, 0);
                auto img_rows = to_int(buffer, 4);
                auto img_cols = to_int(buffer, 8);
                auto img_type = to_int(buffer, 12);
                auto img_size = to_int(buffer, 16);

                auto name = to_container<std::string>(buffer, 20, name_size);
                auto data = to_container<bytes_t>(buffer, 20 + name_size, img_size);

                auto img = ersap::any{Image{cv::Mat{}, std::move(name)}};

                auto& mat = ersap::any_cast<Image&>(img).mat;
                mat.create(img_rows, img_cols, img_type);
                std::copy(data.begin(), data.end(), mat.data);

                return img;
            }

        private:
            int to_int(const bytes_t& b, size_t i) const
            {
                return (b[i+3] << 24) | (b[i+2] << 16) | (b[i+1] << 8) | (b[i+0]);
            }

            template<typename T>
            T to_container(const bytes_t& b, size_t offset, size_t length) const
            {
                return T(b.begin() + offset, b.begin() + offset + length);
            }
        };

        const ersap::EngineDataType SAMPA_DAS{"binary/data-sampa",
                                               std::make_unique<ImageSerializer>()};

    } // end namespace demo
} // end namespace ersap

