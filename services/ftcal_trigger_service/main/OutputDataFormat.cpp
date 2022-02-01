
// Copyright 2022, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.

#include "OutputDataFormat.hpp"


std::vector<std::uint8_t> FTCalTriggerEventSerializer::write(const ersap::any& data) const {
    auto message = ersap::any_cast<FTCalTriggerEvent>(data);
    size_t buffer_size = 0;
    std::vector<std::uint8_t> result;
    result.reserve(buffer_size);
    return result;
}

ersap::any FTCalTriggerEventSerializer::read(const std::vector<std::uint8_t>& buffer) const
{
    FTCalTriggerEvent result;
    return result;
}
