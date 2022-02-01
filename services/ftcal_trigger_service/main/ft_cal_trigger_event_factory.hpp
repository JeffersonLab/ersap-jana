
// Copyright 2022, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.


#ifndef ERSAP_JANA_FT_CAL_TRIGGER_EVENT_FACTORY_HPP
#define ERSAP_JANA_FT_CAL_TRIGGER_EVENT_FACTORY_HPP

#include <JANA/JFactoryT.h>
#include "OutputDataFormat.hpp"

class FTCalTriggerEvent_factory : public JFactoryT<FTCalTriggerEvent> {

public:
    FTCalTriggerEvent_factory() {
        SetFactoryFlag(JFactory_Flags_t::NOT_OBJECT_OWNER);
        // Ownership of these pointers belongs to JANA/ERSAP glue
        // TODO: Serialize inside here so that there is no ownership confusion going forward
    }

    void Process(const std::shared_ptr<const JEvent> &) override;

};


#endif //ERSAP_JANA_FT_CAL_TRIGGER_EVENT_FACTORY_HPP
