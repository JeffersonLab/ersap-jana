#include "group_event_processor.hpp"
#include <vector>

void GroupedEventProcessor::Process(const std::shared_ptr<const JEvent>& event) {

	auto group = event->GetSingle<JEventGroup>();

	// Sequentially, process each event and report when a group finishes
	std::lock_guard<std::mutex> lock(m_mutex);

	LOG << "Processed group #" << group->GetGroupId() << ", event #" << event->GetEventNumber() << LOG_END;

	bool finishes_group = group->FinishEvent();
	if (finishes_group) {
		LOG << "Processed last element in group " << group->GetGroupId() << LOG_END;
	}
}

