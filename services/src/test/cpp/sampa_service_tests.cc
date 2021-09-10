
// Copyright 2021, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.

#include <gtest/gtest.h>
#include "ersap_event_processor.hpp"
#include "sampa_data_type.hpp"
#include "sampa_output_type.hpp"
#include "sampa_test_factory.hpp"
#include "sampa_service.hpp"

#include <JANA/JFactoryGenerator.h>

using namespace ersap::jana;

TEST(SampaServiceTests, BasicAssertions) {

    SampaDASMessage original;
    original.payload = {{1,2,3}, {7,8,9,10}};
    original.timeslice_id = 22;

    auto sut = SampaService();
    ersap::EngineData ed;
    // TODO: Figure out how to insert and retrieve data + metadata from EngineData object
    auto result = sut.execute(ed);

    // TODO: Decide on what to test. Maybe ed should contain the sum of all values in original.payload
    EXPECT_EQ(1,1);

}

TEST(SampaServiceTests, ErsapEventGroupFunctionality) {

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

}

TEST(SampaServiceTests, ErsapEventGroupMultiple) {

    std::vector<SampaDASMessage*> inputs;
    inputs.push_back(new SampaDASMessage);
    inputs.push_back(new SampaDASMessage);
    inputs.push_back(new SampaDASMessage);
    inputs[0]->payload = {{1,2,3}};
    inputs[1]->payload = {{1}};
    inputs[2]->payload = {{22,100}};

    auto app = new JApplication();
    auto src = new ErsapEventSource<SampaDASMessage, SampaOutputMessage>("TestingEventSource", app);
    app->Add(src);
    app->Add(new ErsapEventProcessor<SampaOutputMessage>());
    app->Add(new JFactoryGeneratorT<SampaTestFactory>);
    app->Run(false);

    auto actual_outputs = src->SubmitAndWait(inputs);
    EXPECT_EQ(actual_outputs[0]->sum, 6);
    EXPECT_EQ(actual_outputs[1]->sum, 1);
    EXPECT_EQ(actual_outputs[2]->sum, 122);

}

TEST(SampaServiceTests, ErsapEventGroupWithTags) {

    std::vector<SampaDASMessage*> inputs;
    inputs.push_back(new SampaDASMessage);
    std::vector<SampaOutputMessage*> expected_outputs;
    expected_outputs.push_back(new SampaOutputMessage);

    inputs[0]->payload = {{1,2,3}};
    expected_outputs[0]->sum = 6;

    auto app = new JApplication();
    app->SetParameterValue("factory_input_tag", "silly");
    auto src = new ErsapEventSource<SampaDASMessage, SampaOutputMessage>("TestingEventSource", app, "silly", "sillier");
    app->Add(src);
    app->Add(new JFactoryGeneratorT<SampaTestFactory>("sillier"));
    app->Add(new ErsapEventProcessor<SampaOutputMessage>("sillier"));
    app->Run(false);

    auto actual_outputs = src->SubmitAndWait(inputs);
    EXPECT_EQ(expected_outputs[0]->sum, actual_outputs[0]->sum);

}