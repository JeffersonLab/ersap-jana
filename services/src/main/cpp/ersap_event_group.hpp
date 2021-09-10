
// Copyright 2021, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.


#ifndef ERSAP_JANA_ERSAPEVENTGROUP_H
#define ERSAP_JANA_ERSAPEVENTGROUP_H

#include <JANA/JObject.h>

template <typename InputT, typename OutputT>
class ErsapEventGroup : public JObject {

    mutable std::atomic_int m_events_in_flight {0};
    mutable std::atomic_bool m_group_closed {false};
    mutable std::vector<const InputT*> m_input_data;
    mutable std::map<const InputT*, const OutputT*> m_finished_data;

public:
    /// Record that another event belonging to this group has been emitted.
    /// This is meant to be called from JEventSource::GetEvent.
    void StartEvent(const InputT* event) const {
        m_input_data.push_back(event);
        m_events_in_flight.fetch_add(1);
        m_group_closed = false;
    }

    /// Indicate that no more events in the group are on their way. Note that groups can be re-opened
    /// by simply emitting another event tagged according to that group.
    /// This is meant to be called from JEventSource::GetEvent.
    void CloseGroup() const {
        m_group_closed = true;
    }

    /// Report an event as finished. If this was the last event in the group, IsGroupFinished will now return true.
    /// Please only call once per event, so that we don't have to maintain a set of outstanding event ids.
    /// This takes advantage of C++ atomics to detect if _we_ were the one who finished the whole group without
    /// needing a lock.
    /// This is meant to be called from JEventProcessor::Process.
    bool FinishEvent(const InputT* input, const OutputT* output) const {

        m_finished_data.insert({input, output}); // TODO: not thread safe
        auto prev_events_in_flight = m_events_in_flight.fetch_sub(1);
        assert(prev_events_in_flight > 0); // detect if someone is miscounting
        return (prev_events_in_flight == 1) && m_group_closed;
    }

    /// Test whether all events in the group have finished. Two conditions have to hold:
    /// 1. The number of in-flight events must be zero
    /// 2. The group must be closed. Otherwise, if the JEventSource is slow but the JEventProcessor is fast,
    ///    the number of in-flight events could drop to zero before the group is conceptually finished.
    /// This is meant to be callable from any JANA component.
    /// Note that this doesn't indicate anything about _who_
    bool IsGroupFinished() const {
        return m_group_closed && (m_events_in_flight == 0);
    }

    /// Block until every event in this group has finished, and the eventsource has declared the group closed.
    /// This is meant to be callable from any JANA component.
    std::vector<const OutputT*> WaitUntilFinished() {
        while (!(m_group_closed && (m_events_in_flight == 0))) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        std::vector<const OutputT*> output_data;
        // Enforce that outputs are ordered corresponding to the inputs
        for (auto input : m_input_data) {
            output_data.push_back(m_finished_data[input]);
        }
        return output_data;

        // TODO: Should ErsapEventGroup retain m_output_data pointers?
        //       ErsapEventGroup CANNOT own these things since it's lifespan ends with JES::FinishEvent().
        //       However, what if there are multiple callers to WaitUntilFinished?
        //       Better options available to us:
        //           1. use promises/futures to ensure unique ownership of results
        //           2. wrap m_output_data in shared_ptrs.
        //           3. allow exactly one call to WaitUntilFinished()
    }
};



#endif //ERSAP_JANA_ERSAPEVENTGROUP_H
