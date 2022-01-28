
// Copyright 2021, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.


#ifndef ERSAP_JANA_SAMPACALIBFACTORY_H
#define ERSAP_JANA_SAMPACALIBFACTORY_H

#include "sampa_data_type.hpp"
#include <JANA/JFactoryT.h>

using namespace ersap::jana;

class SampaCalibFactory : public JFactoryT<SampaDASMessage> {
    uint8_t offset = 0;

public:
    SampaCalibFactory();
    void Init() override;
    void Process(const std::shared_ptr<const JEvent>& event) override;
};


#endif //ERSAP_JANA_SAMPACALIBFACTORY_H
