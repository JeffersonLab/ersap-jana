
// Copyright 2021, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.

#include <gtest/gtest.h>
#include "sampa_data_type.hpp"
#include "sampa_service.hpp"

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

