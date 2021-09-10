#include "jana_service.hpp"

#include <jana_engine.hpp>

#include <ersap/stdlib/json_utils.hpp>

#include <cmath>
#include <iostream>

#include <chrono>
#include <sys/time.h>
#include <ctime>

extern "C"
std::unique_ptr<ersap::Engine> create_engine()
{
    return std::make_unique<ersap::jana::JanaService>();
}


namespace ersap {
namespace jana {

    time_t start;

ersap::EngineData JanaService::configure(ersap::EngineData& input)
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

    // TODO: Extract jana_config_file_name from clara config
    auto jana_config_file_name = "config.jana";

    std::cout << "Initializing JApplication" << std::endl;fflush(stdout);
    // std::cout << "Loading options from " << jana_config_file_name << std::endl;fflush(stdout);
    // auto params = new JParameterManager;
    // try {
    //     params->ReadConfigFile(jana_config_file_name);
    // } catch (JException& e) {
    //     std::cout << "Problem reading config file '" << jana_config_file_name << "'. Exiting." << std::endl << std::endl;
    //     exit(-1);
    // }

    m_app = new JApplication(); // (params)
    m_evtsrc = new ClaraEventSource("ClaraEventSource", m_app);

    m_app->Add(m_evtsrc);
    m_app->Add(new GroupedEventProcessor()); // TODO: Move this into ClaraEventSource::Finish() hopefully
    // Add any event processors you might need

    m_app->Run(false);  // Exit immediately, DON'T block until finished
    return {};
}


ersap::EngineData JanaService::execute(ersap::EngineData& input) {
    auto output = ersap::EngineData{};

    time_t end = time(nullptr);

    if (end - start >= 10) {
        // This always loads the shared_pointer into a new shared_ptr
    std::atomic_load(&engine_)->process();
    start = end;
}
    // Set and return output data
//    output.set_data(IMAGE_TYPE, img);
    return input;
}

ersap::EngineData JanaService::execute_group(const std::vector<ersap::EngineData>&)
{
    return {};
}

std::vector<ersap::EngineDataType> JanaService::input_data_types() const
{
    return { ersap::type::JSON, ersap::type::BYTES };
}


std::vector<ersap::EngineDataType> JanaService::output_data_types() const
{
    return { ersap::type::JSON, ersap::type::BYTES };
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
