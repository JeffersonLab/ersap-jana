
// Copyright 2022, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.


#include <ersap/stdlib/json_utils.hpp>
#include <JANA/JFactoryGenerator.h>
#include <JANA/Calibrations/JCalibrationManager.h>
#include <JANA/Calibrations/JCalibrationGeneratorCCDB.h>

#include "ftcal_trigger_service.hpp"
#include "InputDataFormat.hpp"
#include "OutputDataFormat.hpp"
#include "ft_cal_trigger_event_factory.hpp"
#include "ersap_event_processor.hpp"

#include "FT/FTCalCluster_factory.h"
#include "FT/FTCalCluster_factory_km.h"
#include "FT/FTCalCluster_factory_hdbscan.h"
#include "FT/FTCalHit_factory.h"
#include "DAQ/fa250VTPMode7Hit_factory.h"
#include "DAQ/faWaveboardHit_factory.h"
#include "TT/TranslationTable_factory.h"


ersap::EngineData FTCalTriggerService::configure(ersap::EngineData& input)
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
    m_app->SetParameterValue("jana:calib_url", "mysql://clas12reader@clasdb.jlab.org/clas12");
    m_app->SetParameterValue("runtype", "HALLB");  // TODO: These should be done more generically

    m_evtsrc = new ErsapEventSource<TridasEvent, FTCalTriggerEvent>("CLAS12-FTCal-Trigger-EventSource", m_app, "", "calibrated");

    m_app->Add(m_evtsrc);
    m_app->Add(new JFactoryGeneratorT<FTCalCluster_factory>);
    m_app->Add(new JFactoryGeneratorT<FTCalCluster_factory_hdbscan>);
    m_app->Add(new JFactoryGeneratorT<FTCalCluster_factory_km>);
    m_app->Add(new JFactoryGeneratorT<FTCalHit_factory>);
    m_app->Add(new JFactoryGeneratorT<FTCalTriggerEvent_factory>);
    m_app->Add(new JFactoryGeneratorT<TranslationTable_factory>);
    m_app->Add(new JFactoryGeneratorT<fa250VTPMode7Hit_factory>);
    m_app->Add(new JFactoryGeneratorT<faWaveboardHit_factory>);
    m_app->Add(new ErsapEventProcessor<FTCalTriggerEvent>());

    auto calib_manager = std::make_shared<JCalibrationManager>();
    calib_manager->AddCalibrationGenerator(new JCalibrationGeneratorCCDB);
    m_app->ProvideService(calib_manager);

    m_app->Run(false);  // Exit immediately, DON'T block until finished
    return {};
}


ersap::EngineData FTCalTriggerService::execute(ersap::EngineData& input) {
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


ersap::EngineData FTCalTriggerService::execute_group(const std::vector<ersap::EngineData>& inputs)
{
    return {};
}

std::vector<ersap::EngineDataType> FTCalTriggerService::input_data_types() const
{
    return { TRIDAS_EVENT };
}


std::vector<ersap::EngineDataType> FTCalTriggerService::output_data_types() const
{
    return { FTCAL_TRIGGER };
}


std::set<std::string> FTCalTriggerService::states() const
{
    return std::set<std::string>{};
}


std::string FTCalTriggerService::name() const
{
    return "FTCalTriggerService";
}


std::string FTCalTriggerService::author() const
{
    return "Nathan Brei";
}


std::string FTCalTriggerService::description() const
{
    return "Jana based FTCal trigger service example";
}


std::string FTCalTriggerService::version() const
{
    return "0.1";
}
