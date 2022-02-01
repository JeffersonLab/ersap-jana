
// Copyright 2022, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.


#include <ersap/stdlib/json_utils.hpp>
#include <JANA/JFactoryGenerator.h>
#include "ft_trigger_service.hpp"
#include "InputDataFormat.hpp"
#include "OutputDataFormat.hpp"
#include "ersap_event_processor.hpp"

#include "FT/FTCalCluster_factory.h"
#include "FT/FTCalCluster_factory_km.h"
#include "FT/FTCalCluster_factory_hdbscan.h"
#include "FT/FTCalHit_factory.h"


ersap::EngineData FTTriggerService::configure(ersap::EngineData& input)
{
    // Ersap provides a simple JSON parser to read configuration data
    // and configure the service.
    auto config = ersap::stdlib::parse_json(input);

    // TODO: Extract jana_config_file_name from ersap config
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
    m_evtsrc = new ErsapEventSource<TridasEvent, FTCalTriggerEvent>("CLAS12-FTCal-Trigger-EventSource", m_app, "", "calibrated");

    m_app->Add(m_evtsrc);
    m_app->Add(new JFactoryGeneratorT<FTCalCluster_factory>);
    m_app->Add(new JFactoryGeneratorT<FTCalCluster_factory_hdbscan>);
    m_app->Add(new JFactoryGeneratorT<FTCalCluster_factory_km>);
    m_app->Add(new JFactoryGeneratorT<FTCalHit_factory>);
    m_app->Add(new ErsapEventProcessor<FTCalTriggerEvent>());

    m_app->Run(false);  // Exit immediately, DON'T block until finished
    return {};
}


ersap::EngineData FTTriggerService::execute(ersap::EngineData& input) {
    auto output = ersap::EngineData{};

    // If the mime-type is not supported, return an error.
    if (input.mime_type() != TRIDAS_EVENT) {
        output.set_status(ersap::EngineStatus::ERROR);
        output.set_description("Wrong input type");
        return output;
    }

    auto& tridas_event = ersap::data_cast<TridasEvent>(input);
    // We pass JANA a non-owning pointer to each.

    std::vector<TridasEvent*> tridas_events;
    tridas_events.push_back(&tridas_event);

    // TODO: Create an EventGroup optimized for exactly one event?
    // time_t end = time(nullptr);
    // if (end - start >= 1) {
    //     // This always loads the shared_pointer into a new shared_ptr
    //     std::atomic_load(&engine_)->process();
    //     start = end;
    // }
    auto outputs = m_evtsrc->SubmitAndWait(tridas_events);

    // Set and return output data
    output.set_data(FTCAL_TRIGGER, outputs);
    return output;
}


ersap::EngineData FTTriggerService::execute_group(const std::vector<ersap::EngineData>& inputs)
{
    // TODO: Talk to Vardan about
    return {};
}

std::vector<ersap::EngineDataType> FTTriggerService::input_data_types() const
{
    // TODO: Need to understand
    // return { ersap::type::JSON, ersap::type::BYTES };
    return { ersap::type::JSON, TRIDAS_EVENT };
}


std::vector<ersap::EngineDataType> FTTriggerService::output_data_types() const
{
    // TODO: Need to understand
    // return { ersap::type::JSON, ersap::type::BYTES };
    return {  };
}


std::set<std::string> FTTriggerService::states() const
{
    return std::set<std::string>{};
}


std::string FTTriggerService::name() const
{
    return "FTTriggerService";
}


std::string FTTriggerService::author() const
{
    return "Nathan Brei";
}


std::string FTTriggerService::description() const
{
    return "Jana based FTCal trigger service example";
}


std::string FTTriggerService::version() const
{
    return "0.1";
}
