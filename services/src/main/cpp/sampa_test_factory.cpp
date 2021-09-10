
// Copyright 2021, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.


#include "sampa_test_factory.hpp"

#include "sampa_data_type.hpp"
#include "sampa_output_type.hpp"
#include <JANA/JEvent.h>
#include <numeric>


SampaTestFactory::SampaTestFactory() {
    SetFactoryFlag(NOT_OBJECT_OWNER);
}

void SampaTestFactory::Init()  {
    GetApplication()->GetParameter("factory_input_tag", input_tag);
}

void SampaTestFactory::Process(const std::shared_ptr<const JEvent> &event) {

    auto summary = new SampaOutputMessage();
    auto sampa_message = event->GetSingle<ersap::jana::SampaDASMessage>(input_tag);
    if (!sampa_message->payload.empty()) {
        summary->sum = std::accumulate(std::begin(sampa_message->payload[0]), std::end(sampa_message->payload[0]), 0);
    }
    LOG << "Setting sum=" << summary->sum << LOG_END;
    Insert(summary);

}
