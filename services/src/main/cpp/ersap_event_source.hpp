//
// Created by Nathan Brei on 2019-12-15.
//

#ifndef JANA2_ErsapEventSource_H
#define JANA2_ErsapEventSource_H

#include <JANA/JEventSource.h>
#include <JANA/JEvent.h>

#include <queue>
#include "sampa_data_type.hpp"
#include "sampa_output_type.hpp"
#include "ersap_event_group.hpp"

using namespace ersap::jana;

class ErsapEventSource : public JEventSource {

    std::mutex m_pending_mutex;
    std::queue<std::pair<SampaDASMessage*, ErsapEventGroup<SampaDASMessage, SampaOutputMessage>*>> m_pending_events;

public:

    ErsapEventSource(std::string res_name, JApplication* app);

    static std::string GetDescription() { return "ERSAP,SAMPA -> JANA Event Source"; }


    /// SubmitAndWait provides a blocking interface for pushing groups of SampaDASMessages into JANA.
    /// JANA does NOT assume ownership of the events vector, nor does it clear it. Note: This must only
    /// be called once, because ownership of the SampaOutputMessages transfers to the caller.
    std::vector<const SampaOutputMessage *> SubmitAndWait(std::vector<SampaDASMessage*>& events);


    /// GetEvent polls the queue of submitted SampaDASMessages and feeds them into JEvents.
    void GetEvent(std::shared_ptr<JEvent> event) override;


    /// FinishEvent is the final method that gets called to close out each JEvent before it gets recycled.
    /// This is where we report our event as being finished. Once all events in the eventgroup are finished,
    /// the corresponding call to SubmitAndWait will unblock.
    void FinishEvent(JEvent& event) override;

};


#endif //JANA2_ErsapEventSource_H
