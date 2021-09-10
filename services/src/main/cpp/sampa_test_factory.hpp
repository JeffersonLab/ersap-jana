
// Copyright 2021, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.


#ifndef ERSAP_JANA_SAMPATESTFACTORY_H
#define ERSAP_JANA_SAMPATESTFACTORY_H

#include "sampa_output_type.hpp"
#include <JANA/JFactoryT.h>

class SampaTestFactory : public JFactoryT<SampaOutputMessage> {
public:
    SampaTestFactory();
    void Process(const std::shared_ptr<const JEvent>& aEvent) override;
};


#endif //ERSAP_JANA_SAMPATESTFACTORY_H
