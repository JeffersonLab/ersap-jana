
// Copyright 2021, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.

#include <gtest/gtest.h>
#include "sampa_data_type.cpp"

using namespace ersap::jana;

TEST(SampaSerializerTests, BasicAssertions) {

    SampaDASMessage original;
    original.payload = {{1,2,3}, {7,8,9,10}};

    auto sut = SampaSerializer();
    auto serialized = sut.write(original);
    auto deserialized = ersap::any_cast<SampaDASMessage>(sut.read(serialized));

    EXPECT_EQ(original.payload, deserialized.payload);

}

