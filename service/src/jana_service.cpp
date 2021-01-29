//
// Created by Vardan Gyurjyan on 1/28/21.
//

#include "jana_service.hpp"
#include <clara/stdlib/json_utils.hpp>

#include <cmath>
#include <iostream>

extern "C"
std::unique_ptr<clara::Engine> create_engine() {
    return std::make_unique<clara::jana::JanaService>();
}
namespace clara {
    namespace jana {

        clara::EngineData JanaService::configure(clara::EngineData &input) {
            // Clara provides a simple JSON parser to read configuration data
            // and configure the service.
            auto config = clara::stdlib::parse_json(input);

            // Example for when the service has state that is configured by
            // the orchestrator. The "state" object should be a std::shared_ptr
            // accessed atomically.
            //
            // (This service is actually stateless, so engine_ could just simply be
            // initialized in the service constructor).
//            std::atomic_store(&engine_, std::make_shared<JanaEngine>());
            return {};
        }

        clara::EngineData JanaService::execute(clara::EngineData &input) {
            auto output = clara::EngineData{};
            return output;
        }

        clara::EngineData JanaService::execute_group(const std::vector<clara::EngineData> &) {
            return {};
        }


        std::vector<clara::EngineDataType> JanaService::input_data_types() const {
            return {clara::type::STRING};
        }


        std::vector<clara::EngineDataType> JanaService::output_data_types() const {
            return {clara::type::STRING};
        }


        std::set<std::string> JanaService::states() const {
            return std::set<std::string>{};
        }


        std::string JanaService::name() const {
            return "JanaService";
        }


        std::string JanaService::author() const {
            return "Vardan Gyurjyan";
        }


        std::string JanaService::description() const {
            return "Does some processing...";
        }


        std::string JanaService::version() const {
            return "0.1";
        }
    }
}
