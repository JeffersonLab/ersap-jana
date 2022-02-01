
// Copyright 2021, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.

#include <gtest/gtest.h>
#include "ersap_event_processor.hpp"
#include "InputDataFormat.hpp"

#include "ftcal_trigger_service_tests.hpp"


TEST(FTCalTriggerServiceTests, EndToEnd) {

    TridasEvent input;

/*
    std::vector<SampaDASMessage*> inputs;
    inputs.push_back(new SampaDASMessage);
    std::vector<SampaOutputMessage*> expected_outputs;
    expected_outputs.push_back(new SampaOutputMessage);

    inputs[0]->payload = {{1,2,3}};
    expected_outputs[0]->sum = 6;

    auto app = new JApplication();
    auto src = new ErsapEventSource<SampaDASMessage, SampaOutputMessage>("TestingEventSource", app);
    app->Add(src);
    app->Add(new ErsapEventProcessor<SampaOutputMessage>);
    app->Add(new JFactoryGeneratorT<SampaTestFactory>);
    app->Run(false);

    auto actual_outputs = src->SubmitAndWait(inputs);
    EXPECT_EQ(expected_outputs[0]->sum, actual_outputs[0]->sum);
    */

}