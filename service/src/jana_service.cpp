//
// Created by Vardan Gyurjyan on 1/28/21.
//

#include "jana_service.hpp"
#include <ersap/stdlib/json_utils.hpp>

#include <cmath>
#include <iostream>

extern "C"
std::unique_ptr<ersap::Engine> create_engine() {
    return std::make_unique<ersap::jana::JanaService>();
}
namespace ersap {
    namespace jana {

        ersap::EngineData JanaService::configure(ersap::EngineData &input) {
            // Ersap provides a simple JSON parser to read configuration data
            // and configure the service.
            auto config = ersap::stdlib::parse_json(input);

            // Example for when the service has state that is configured by
            // the orchestrator. The "state" object should be a std::shared_ptr
            // accessed atomically.
            //
            // (This service is actually stateless, so engine_ could just simply be
            // initialized in the service constructor).
//            std::atomic_store(&engine_, std::make_shared<JanaEngine>());
//            return {};
            return NULL;
        }

        ersap::EngineData JanaService::execute(ersap::EngineData &input) {
            // auto output = ersap::EngineData{};
            std::cout << "DDD in the c++ engine....... "<< std::endl;
            return input;
        }

        ersap::EngineData JanaService::execute_group(const std::vector<ersap::EngineData> &) {
            return {};
        }


        std::vector<ersap::EngineDataType> JanaService::input_data_types() const {
            return {ersap::type::STRING};
        }


        std::vector<ersap::EngineDataType> JanaService::output_data_types() const {
            return {ersap::type::STRING};
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
