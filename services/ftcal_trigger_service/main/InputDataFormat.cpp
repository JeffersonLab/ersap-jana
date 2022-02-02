
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
    const TEHeaderInfo* header = reinterpret_cast<const TEHeaderInfo*>(pos);
    size_t frame_count = header->nHit;
    pos += sizeof(TEHeaderInfo);
    for (size_t frame_idx = 0; frame_idx < frame_count; ++frame_idx) {
        const DataFrameHeader* frame = reinterpret_cast<const DataFrameHeader*>(pos);
        fadcHit hit;
        hit.crate = frame->TowerID;
        hit.slot = frame->EFCMID;
        hit.channel = frame->PMTID;

        // frame:
        // unsigned int Charge: 16;                 // 0-8191+
        // unsigned int T1ns:   16;                 // 1 ns counter, 0-65536ns

        // fadcHit:
        // typedef std::chrono::duration<boost::int_least64_t, std::ratio<1, 250000000> > T4nsec;
        // T4nsec time;
        // float charge; //Charge corrected by ADC pedestals

        hit.charge = frame->Charge;
        // hit.time = frame->T1ns;
        hit.type = FA250VTPMODE7;
        // TODO:
        result.hits.push_back(hit);
        pos += sizeof(DataFrameHeader);
    }
    return result;
}
