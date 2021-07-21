#include "jana_service.hpp"

//#include <image_data_type.hpp>
#include <jana_engine.hpp>

#include <ersap/stdlib/json_utils.hpp>

#include <cmath>
#include <iostream>

extern "C"
std::unique_ptr<ersap::Engine> create_engine()
{
    return std::make_unique<ersap::jana::JanaService>();
}


namespace ersap {
namespace jana {

ersap::EngineData JanaService::configure(ersap::EngineData& input)
{
    // Ersap provides a simple JSON parser to read configuration data
    // and configure the service.
    auto config = ersap::stdlib::parse_json(input);

    // Example for when the service has state that is configured by
    // the orchestrator. The "state" object should be a std::shared_ptr
    // accessed atomically.
    //
    // (This service is actually stateless, so detector_ could just simply be
    // initialized in the service constructor).
    std::atomic_store(&engine_, std::make_shared<JanaEngine>());
    return {};
}


ersap::EngineData JanaService::execute(ersap::EngineData& input)
{
    auto output = ersap::EngineData{};

    // If the mime-type is not supported, return an error.
//    if (input.mime_type() != IMAGE_TYPE) {
//        output.set_status(ersap::EngineStatus::ERROR);
//        output.set_description("Wrong input type");
//        return output;
//    }
//
//    auto& img = ersap::data_cast<Image>(input);

    // This always loads the shared_pointer into a new shared_ptr
//    std::atomic_load(&detector_)->run(img.mat);
    std::atomic_load(&engine_)->process();

    // Set and return output data
//    output.set_data(IMAGE_TYPE, img);
    return output;
}


ersap::EngineData JanaService::execute_group(const std::vector<ersap::EngineData>&)
{
    return {};
}


std::vector<ersap::EngineDataType> JanaService::input_data_types() const
{
//    return { ersap::type::JSON, IMAGE_TYPE };
    return { ersap::type::JSON};
}


std::vector<ersap::EngineDataType> JanaService::output_data_types() const
{
//    return { ersap::type::JSON, IMAGE_TYPE };
    return { ersap::type::JSON };
}


std::set<std::string> JanaService::states() const
{
    return std::set<std::string>{};
}


std::string JanaService::name() const
{
    return "JanaService";
}


std::string JanaService::author() const
{
    return "Vardan Gyurjyan";
}


std::string JanaService::description() const
{
    return "Jana based service example";
}


std::string JanaService::version() const
{
    return "0.1";
}

}
}
