#include "group_event_processor.hpp"

#include "sampa_data_type.hpp"
#include "sampa_output_type.hpp"
#include "ersap_event_group.hpp"

#include <vector>


void GroupedEventProcessor::Process(const std::shared_ptr<const JEvent>& event) {

	auto group = event->GetSingle<ErsapEventGroup<ersap::jana::SampaDASMessage, SampaOutputMessage>>();

	// In parallel, extract payload and do some calculations on it
	// This should REALLY be in a JFactory, except this is throwaway work for the sake of the test cases

    auto summary = new SampaOutputMessage();
	auto sampa_message = event->GetSingle<ersap::jana::SampaDASMessage>();
	if (!sampa_message->payload.empty()) {
	    summary->sum = std::accumulate(std::begin(sampa_message->payload[0]), std::end(sampa_message->payload[0]), 0);
	}
    LOG << "Setting sum=" << summary->sum << LOG_END;
    event->Insert(summary)->SetFactoryFlag(JFactory::NOT_OBJECT_OWNER); // TODO: Constness doesn't prevent this. Consider.
    // TODO: Need to ASSERT that factory flag is NOT_OBJECT_OWNER because otherwise the use-after-free
    //       will be impossible for the user to debug

	// Sequentially, process each event and report when a group finishes
	std::lock_guard<std::mutex> lock(m_mutex);
}

