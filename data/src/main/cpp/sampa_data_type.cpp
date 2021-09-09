#include "sampa_data_type.hpp"

#include <cstdint>

namespace ersap {
    namespace jana {

        int32_t SampaSerializer::read_int(std::vector<uint8_t>::const_iterator& it) const {
            int32_t result;
            std::copy(it, it+4, reinterpret_cast<uint8_t*>(&result));
            it += 4;
            return result;
        }

        void SampaSerializer::write_int(std::vector<uint8_t>& v, int32_t value) const {
            uint8_t temp[4];
            auto rtemp = reinterpret_cast<int32_t*>(temp);
            *rtemp = value;
            std::copy(std::begin(temp), std::end(temp), std::back_inserter(v));
        }

        std::vector<std::uint8_t> SampaSerializer::write(const ersap::any& data) const {
            auto message = any_cast<SampaDASMessage>(data);
            uint32_t est_buffer_size = (message.payload.size() > 0)
                                            ? message.payload.size()*message.payload[0].size()
                                            : 0;

            std::vector<std::uint8_t> result;
            result.reserve(est_buffer_size);

            write_int(result, message.payload.size());
            for (const auto& ch : message.payload) {
                write_int(result, ch.size());
                std::copy(begin(ch), end(ch), std::back_inserter(result));
            }
            return result;
        }

        ersap::any SampaSerializer::read(const std::vector<std::uint8_t>& buffer) const
        {
            SampaDASMessage result;
            auto position = begin(buffer);
            auto num_channels = read_int(position);
            result.payload.reserve(num_channels);
            for (int i=0; i<num_channels; ++i) {
                auto num_samples = read_int(position);
                std::vector<SampaDASMessage::byte_t> channel(position, position+num_samples);
                result.payload.push_back(channel);
                position += num_samples;
            }
            return result;
        }

        const ersap::EngineDataType SAMPA_DAS{"binary/data-sampa",
                                               std::make_unique<SampaSerializer>()};

    } // end namespace demo
} // end namespace ersap

