
// Copyright 2021, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.


#include "sampa_calib_factory.hpp"
#include <JANA/JEvent.h>

SampaCalibFactory::SampaCalibFactory() {
    SetFactoryFlag(NOT_OBJECT_OWNER);
}

void SampaCalibFactory::Init()  {
    GetApplication()->SetDefaultParameter("sampa_calib_offset", offset);
}

void SampaCalibFactory::Process(const std::shared_ptr<const JEvent> &event) {

    auto input = event->GetSingle<SampaDASMessage>();
    auto output = new SampaDASMessage;

    for (const auto& channel : input->payload) {
        std::vector<uint8_t> calibrated_channel;
        calibrated_channel.reserve(channel.size());
        for (const auto& sample : channel) {
            calibrated_channel.push_back(sample + offset);
        }
        output->payload.push_back(std::move(calibrated_channel));
    }
    Insert(output);
}
