//
// Created by Vardan Gurjyan on 8/25/21.
//
#include "sampa_service.hpp"

#include <sampa_data_type.hpp>
#include <sampa_engine.hpp>

#include <ersap/stdlib/json_utils.hpp>

#include <cmath>
#include <iostream>

#include <chrono>
#include <sys/time.h>
#include <ctime>

extern "C"
std::unique_ptr<ersap::Engine> create_engine()
{
    return std::make_unique<ersap::jana::SampaService>();
}

namespace ersap {
    namespace jana {

        time_t start;

        ersap::EngineData SampaService::configure(ersap::EngineData& input)
        {
            // Ersap provides a simple JSON parser to read configuration data
            // and configure the service.
            auto config = ersap::stdlib::parse_json(input);

            start = time(nullptr);

            // Example for when the service has state that is configured by
            // the orchestrator. The "state" object should be a std::shared_ptr
            // accessed atomically.
            //
            // (This service is actually stateless, so detector_ could just simply be
            // initialized in the service constructor).
            std::atomic_store(&engine_, std::make_shared<JanaEngine>());
            return {};
        }


        ersap::EngineData SampaService::execute(ersap::EngineData& input) {
            auto output = ersap::EngineData{};

            time_t end = time(nullptr);

            if (end - start >= 10) {
                // This always loads the shared_pointer into a new shared_ptr
                std::atomic_load(&engine_)->process();
                start = end;
            }
            return input;
        }

        ersap::EngineData SampaService::execute_group(const std::vector<ersap::EngineData>&)
        {
            return {};
        }

        std::vector<ersap::EngineDataType> SampaService::input_data_types() const
        {
            return { ersap::type::JSON, SAMPA_DAS };
        }


        std::vector<ersap::EngineDataType> SampaService::output_data_types() const
        {
            return { ersap::type::JSON, SAMPA_DAS };
        }


        std::set<std::string> SampaService::states() const
        {
            return std::set<std::string>{};
        }


        std::string SampaService::name() const
        {
            return "SampaService";
        }


        std::string SampaService::author() const
        {
            return "Vardan Gyurjyan";
        }


        std::string SampaService::description() const
        {
            return "Jana based sampa data processing service example";
        }


        std::string SampaService::version() const
        {
            return "0.1";
        }

    }
}

