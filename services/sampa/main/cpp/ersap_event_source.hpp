//
// Created by Nathan Brei on 2019-12-15.
//

#ifndef JANA2_ErsapEventSource_H
#define JANA2_ErsapEventSource_H

#include <JANA/JEventSource.h>
#include <JANA/JEvent.h>

#include <queue>
#include "ersap_event_group.hpp"


template <typename InputT, typename OutputT>
class ErsapEventSource : public JEventSource {

    std::mutex m_pending_mutex;
    std::queue<std::pair<InputT*, ErsapEventGroup<InputT, OutputT>*>> m_pending_events;
    const std::string m_input_tag;
    const std::string m_output_tag;

public:

    ErsapEventSource(std::string res_name, JApplication* app, std::string input_tag="", std::string output_tag="")
    : JEventSource(std::move(res_name), app)
    , m_input_tag(std::move(input_tag))
    , m_output_tag(std::move(output_tag))
    {
        EnableFinishEvent();
    }

    static std::string GetDescription() {
        return "ERSAP Event Source";
    }


    /// SubmitAndWait provides a blocking interface for pushing groups of InputTs into JANA.
    /// JANA does NOT assume ownership of the events vector, nor does it clear it. Note: This must only
    /// be called once, because ownership of the OutputTs transfers to the caller.
    std::vector<const OutputT *> SubmitAndWait(std::vector<InputT*>& events) {
        auto group = new ErsapEventGroup<InputT, OutputT>;
        LOG << "Starting group " << group << LOG_END;
        {
            std::lock_guard<std::mutex> lock(m_pending_mutex);
            for (auto event : events) {
                group->StartEvent(event);   // We have to call this immediately in order to 'open' the group
                m_pending_events.push(std::make_pair(event, group));
            }
        }
        group->CloseGroup();
        auto results = group->WaitUntilFinished();
        delete group;
        return results;
    }


    /// GetEvent polls the queue of submitted InputTs and feeds them into JEvents.
    void GetEvent(std::shared_ptr<JEvent> event) override {

        std::pair<InputT*, ErsapEventGroup<InputT, OutputT>*> next_event;
        {
            std::lock_guard<std::mutex> lock(m_pending_mutex);
            if (m_pending_events.empty()) {
                throw RETURN_STATUS::kTRY_AGAIN;
            } else {
                next_event = m_pending_events.front();
                m_pending_events.pop();
            }
        }

        // Hydrate JEvent with both the SampaEvent and the group pointer.
        event->Insert(next_event.first, m_input_tag);    // SampaEvent
        event->Insert(next_event.second);   // JEventGroup

        // Tell JANA not to assume ownership of these objects!
        event->GetFactory<InputT>(m_input_tag)->SetFactoryFlag(JFactory::JFactory_Flags_t::NOT_OBJECT_OWNER);
        event->GetFactory<ErsapEventGroup<InputT, OutputT>>()->SetFactoryFlag(JFactory::JFactory_Flags_t::NOT_OBJECT_OWNER);

        // JANA always needs an event number and a run number, so extract these from the Tridas data somehow
        event->SetEventNumber(next_event.first->get_event_number());
        event->SetRunNumber(next_event.first->get_run_number());
    }


    /// FinishEvent is the final method that gets called to close out each JEvent before it gets recycled.
    /// This is where we report our event as being finished. Once all events in the eventgroup are finished,
    /// the corresponding call to SubmitAndWait will unblock.
    void FinishEvent(JEvent& event) override {

        auto group = event.GetSingle<ErsapEventGroup<InputT, OutputT>>();
        auto input = event.GetSingle<InputT>(m_input_tag);
        auto output = event.GetSingle<OutputT>(m_output_tag);

        LOG << "Finishing group " << group << ", event " << event.GetEventNumber() << LOG_END;
        bool entire_group_finished = group->FinishEvent(input, output);
        if (entire_group_finished) {
            LOG << "Finishing group " << group << LOG_END;
        }
    }

};


#endif //JANA2_ErsapEventSource_H
