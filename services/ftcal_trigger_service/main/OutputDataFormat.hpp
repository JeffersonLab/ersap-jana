
// Copyright 2022, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.


#ifndef ERSAP_JANA_OUTPUTDATAFORMAT_HPP
#define ERSAP_JANA_OUTPUTDATAFORMAT_HPP

#include "Trigger/TriggerDecision.h"
#include "FT/FTCalCluster.h"

#include <ersap/engine_data_type.hpp>

struct FTCalTriggerEvent {
    std::vector<TriggerDecision*> triggerDecisions;
    std::vector<FTCalHit*> ftcal_hits;
    std::vector<FTCalCluster*> ftcal_clusters;
};

struct FTCalTriggerEventSerializer : public ersap::Serializer {
    std::vector<std::uint8_t> write(const ersap::any& data) const;
    ersap::any read(const std::vector<std::uint8_t>& buffer) const;
};

const ersap::EngineDataType FTCAL_TRIGGER {"binary/ft-cal-trigger-event",
                                           std::make_unique<FTCalTriggerEventSerializer>()};
#endif //ERSAP_JANA_OUTPUTDATAFORMAT_HPP
