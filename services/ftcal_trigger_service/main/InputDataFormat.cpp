
// Copyright 2022, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.

#include "InputDataFormat.hpp"


std::vector<std::uint8_t> TridasEventSerializer::write(const ersap::any& data) const {
    auto message = ersap::any_cast<TridasEvent>(data);
    size_t buffer_size = 0;
    std::vector<std::uint8_t> result;
    result.reserve(buffer_size);
    return result;
}

ersap::any TridasEventSerializer::read(const std::vector<std::uint8_t>& buffer) const
{
    TridasEvent result;
    const uint8_t* pos = buffer.data();
    result.header = *(reinterpret_cast<const TEHeaderInfo*>(pos));
    size_t frame_count = result.header.nHit;
    pos += sizeof(TEHeaderInfo);
    for (size_t frame = 0; frame < frame_count; ++frame) {
        result.dataframes.push_back(*(reinterpret_cast<const DataFrameHeader*>(pos)));
        pos += sizeof(DataFrameHeader);
    }
    return result;
}
