//
// Created by Nathan Brei on 2019-12-15.
//

#ifndef JANA2_ErsapEventSource_H
#define JANA2_ErsapEventSource_H

#include <JANA/JEventSource.h>
#include <JANA/Services/JEventGroupTracker.h>
#include <JANA/JEvent.h>

#include <queue>
#include "sampa_data_type.hpp"

using namespace ersap::jana;

class ErsapEventSource : public JEventSource {

    JEventGroupManager m_egm;

    int m_pending_group_id;
    std::mutex m_pending_mutex;
    std::queue<std::pair<SampaDASMessage*, JEventGroup*>> m_pending_events;

public:

    ErsapEventSource(std::string res_name, JApplication* app);

    static std::string GetDescription() { return "CLARA,SAMPA -> JANA Event Source"; }


    /// SubmitAndWait provides a blocking interface for pushing groups of TridasEvents into JANA.
    /// JANA does NOT assume ownership of the events vector, nor does it clear it.
    /// TODO: who clears events vector?
    void SubmitAndWait(std::vector<SampaDASMessage*>& events);


    /// GetEvent polls the queue of submitted TridasEvents and feeds them into JEvents along with a
    /// JEventGroup. A downstream EventProcessor may report the event as being finished. Once all
    /// events in the eventgroup are finished, the corresponding call to SubmitAndWait will unblock.
    void GetEvent(std::shared_ptr<JEvent> event) override;

};


#endif //JANA2_ErsapEventSource_H
