
// Copyright 2022, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.


#ifndef ERSAP_JANA_OUTPUTDATAFORMAT_HPP
#define ERSAP_JANA_OUTPUTDATAFORMAT_HPP

#include "Trigger/TriggerDecision.h"
#include "FT/FTCalCluster.h"

// TODO: I have no idea what this should be, probably EVIO, but I don't know how to do that yet,
// so instead I'm going to defer the decision on how to serialize until later

struct ProcessedEvent {
    std::vector<TriggerDecision*> triggerDecisions;
    std::vector<FTCalHit*> ftcal_hits;
    std::vector<FTCalCluster*> ftcal_clusters;
};

#endif //ERSAP_JANA_OUTPUTDATAFORMAT_HPP
