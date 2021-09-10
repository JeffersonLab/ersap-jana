//
// Created by Nathan Brei on 2019-12-15.
//

#ifndef JANA2_GROUPEDEVENTPROCESSOR_H
#define JANA2_GROUPEDEVENTPROCESSOR_H

#include <JANA/JEventProcessor.h>
#include <cassert>


template <typename InputT, typename OutputT>
class GroupedEventProcessor : public JEventProcessor {

    const std::string helpstr = "Factory must relinquish ownership of its objects by setting the NOT_OBJECT_OWNER flag.  \n"
                                "Otherwise JANA will delete the objects automatically when it recycles the JEvent, leading \n"
                                "to a use-after-free bug when you try to access the output data.";

public:
    void Process(const std::shared_ptr<const JEvent>& event) override {

        // In parallel, trigger the recursive factory calls to build the output
        event->GetSingle<OutputT>();

        // Ensure the user isn't immediately going to trigger a use-after-free
        assert((helpstr, event->GetFactory<OutputT>("")->TestFactoryFlag(JFactory::NOT_OBJECT_OWNER)));
    }
};




#endif //JANA2_GROUPEDEVENTPROCESSOR_H
