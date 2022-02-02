
// Copyright 2022, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.


#ifndef ERSAP_JANA_FTCAL_TRIGGER_SERVICE_HPP
#define ERSAP_JANA_FTCAL_TRIGGER_SERVICE_HPP

#include <ersap/engine.hpp>
#include <JANA/JApplication.h>

#include "DAQ/TridasEvent.h"
#include "InputDataFormat.hpp"
#include "OutputDataFormat.hpp"
#include "ersap_event_source.hpp"


class FTCalTriggerService : public ersap::Engine
{
public:
    FTCalTriggerService() = default;

    FTCalTriggerService(const FTCalTriggerService&) = delete;
    FTCalTriggerService& operator=(const FTCalTriggerService&) = delete;

    FTCalTriggerService(FTCalTriggerService&&) = default;
    FTCalTriggerService& operator=(FTCalTriggerService&&) = default;

    ~FTCalTriggerService() override = default;

public:
    ersap::EngineData configure(ersap::EngineData&) override;

    ersap::EngineData execute(ersap::EngineData&) override;

    ersap::EngineData execute_group(const std::vector<ersap::EngineData>&) override;

public:
    std::vector<ersap::EngineDataType> input_data_types() const override;

    std::vector<ersap::EngineDataType> output_data_types() const override;

    std::set<std::string> states() const override;

public:
    std::string name() const override;

    std::string author() const override;

    std::string description() const override;

    std::string version() const override;

private:
    // TODO: Use smart pointers here
    // TODO: Deal with multiple threads calling configure() and clobbering JApplication
    JApplication* m_app;
    ErsapEventSource<TridasEvent, FTCalTriggerEvent>* m_evtsrc;
    // std::shared_ptr<SampaEngine> engine_{};
};
class clas12_ft_trigger_service {

};


#endif //ERSAP_JANA_FTCAL_TRIGGER_SERVICE_HPP
