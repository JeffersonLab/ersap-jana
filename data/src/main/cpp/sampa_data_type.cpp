#include "sampa_data_type.hpp"

#include <cstdint>
#include <iostream>

namespace ersap {
    namespace jana {


        class SampaSerializer : public ersap::Serializer
        {
        public:
            std::vector<std::uint8_t> write(const ersap::any& data) const override
            {
            }

            ersap::any read(const std::vector<std::uint8_t>& buffer) const override
            {
            }

        };

        const ersap::EngineDataType SAMPA_DAS{"binary/data-sampa",
                                               std::make_unique<SampaSerializer>()};

    } // end namespace demo
} // end namespace ersap

