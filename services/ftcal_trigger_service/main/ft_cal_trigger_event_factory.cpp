
// Copyright 2022, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.


#include "ft_cal_trigger_event_factory.hpp"
#include <JANA/JEvent.h>

void FTCalTriggerEvent_factory::Process(const std::shared_ptr<const JEvent>& event) {

    FTCalTriggerEvent* output_event = new FTCalTriggerEvent;
    output_event->ftcal_hits = event->Get<FTCalHit>();
    output_event->ftcal_clusters = event->Get<FTCalCluster>();
    output_event->triggerDecisions = event->Get<TriggerDecision>();
    // TODO: Is this enough to distinguish TriggerDecision subtypes?

    Set({output_event});
    // event->Get<

}

