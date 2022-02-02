
// Copyright 2022, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.


#ifndef ERSAP_JANA_INPUTDATAFORMAT_HPP
#define ERSAP_JANA_INPUTDATAFORMAT_HPP

#include <ersap/engine_data_type.hpp>
#include "DAQ/TridasEvent.h"

const unsigned int MAX_PLUGINS_NUMBER  = 8;
const unsigned int MAX_TRIGGERS_NUMBER = 5;
typedef size_t TS_t;

struct TimeSliceHeader {
    TS_t TS_ID;             // ID number of the TimeSlice
    uint32_t NEvents;       // Number of events in the TimeSlice
    uint32_t TS_size;       // size of the whole TS data
    // (TimeSliceHeader + events payload, bytes)
    uint32_t missed_frames; // Number of lost frames during the time slice
};

struct TEHeaderInfo {
    uint32_t EventTag;    // Tag for HeaderE = 12'08'1972; magic number
    uint32_t EventID;     // ID - to be set by the EM
    uint32_t EventL;      // total length of the TE (header included)
    uint32_t nHit;        // n tot. hits
    uint64_t StartTime;   // Start time in 1ns resolution
    uint32_t TSCompleted; // flag for event belonging to an (un)completed (0)1 TS

    unsigned int nseeds[MAX_TRIGGERS_NUMBER];         // array that store the number of
    // seeds per trigger type
    unsigned int plugin_trigtype[MAX_PLUGINS_NUMBER]; // array - seed-found flags
    // for each kind of plugin
    unsigned int plugin_nseeds[MAX_PLUGINS_NUMBER];   // array - nseeds for each
    // kind of L2 algorithm
};

#pragma pack(push, 1)
struct DataFrameHeader {
    unsigned int PMTID:   4;                 // 0-15;  -> ChannelID
    unsigned int EFCMID:  5;                 // 0-31;  -> SlotID
    unsigned int TowerID: 7;                 // 0-127; -> CrateID
    unsigned int Charge: 16;                 // 0-8191+
    unsigned int T1ns:   16;                 // 1 ns counter, 0-65536ns
    unsigned long long int FrameCounter: 48; // 65536ns counter, 0-584.5y
};
#pragma pack(pop)



struct TridasEventSerializer : public ersap::Serializer {
    std::vector<std::uint8_t> write(const ersap::any& data) const;
    ersap::any read(const std::vector<std::uint8_t>& buffer) const;
};

const ersap::EngineDataType TRIDAS_EVENT {"binary/tridas-event",
                                      std::make_unique<TridasEventSerializer>()};



#endif //ERSAP_JANA_INPUTDATAFORMAT_HPP
