#ifndef ERSAP_JANA_SAMPA_DATA_TYPE_HPP
#define ERSAP_JANA_SAMPA_DATA_TYPE_HPP

#include <ersap/engine_data_type.hpp>
// #include <chrono>

namespace ersap {
    namespace jana {

        struct SampaDASMessage {

            using byte_t = std::uint8_t;
            std::vector<std::vector<byte_t>> payload;

            // We don't need these right this moment but eventually we might
            // Ersap thinks of these as "metadata" but they still need to be made available to JANA

            // uint64_t timeslice_id;
            // std:chrono::time_point<std::chrono::steady_clock> timeslice_timestamp;
            // uint32_t channel_id_offset;
        };

        class SampaSerializer : public ersap::Serializer {

        private:
            int32_t read_int(std::vector<uint8_t>::const_iterator& it) const;
            void write_int(std::vector<uint8_t>& v, int32_t value) const;

        public:
            std::vector<std::uint8_t> write(const ersap::any& data) const override;
            ersap::any read(const std::vector<std::uint8_t>& buffer) const override;
    };

    const extern ersap::EngineDataType SAMPA_DAS;

    } // end namespace jana

} // end namespace ersap

#endif //ERSAP_JANA_SAMPA_DATA_TYPE_HPP
