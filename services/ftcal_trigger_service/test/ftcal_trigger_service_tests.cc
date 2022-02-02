
// Copyright 2021, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.

#include <gtest/gtest.h>
#include <ersap/engine_data.hpp>
#include <ersap/engine_data_type.hpp>
#include "ersap_event_processor.hpp"
#include "ftcal_trigger_service.hpp"

#include "ftcal_trigger_service_tests.hpp"


TEST(FTCalTriggerServiceTests, EndToEndZeroHits) {

    TridasEvent input;
    // No hits in this Event

    ersap::EngineData ersap_input;
    ersap_input.set_data(TRIDAS_EVENT, input);
    FTCalTriggerService sut;
    ersap::EngineData ersap_config;
    ersap_config.set_data(ersap::type::JSON, "{}");
    sut.configure(ersap_config);
    auto ersap_output = sut.execute(ersap_input);
    FTCalTriggerEvent output = ersap::data_cast<FTCalTriggerEvent>(ersap_output);

    EXPECT_EQ(output.ftcal_hits.size(), 0);

}