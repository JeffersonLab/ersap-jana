//
// Created by Nathan Brei on 2019-12-15.
//

#ifndef JANA2_GROUPEDEVENTPROCESSOR_H
#define JANA2_GROUPEDEVENTPROCESSOR_H

#include <JANA/JEventProcessor.h>
#include <cassert>


/// ErsapEventProcessor's job is to ensure that the factory for producing the OutputT gets called in parallel.
/// It also checks that the factory is not claiming ownership over said objects, which would cause a use-after-free bug.
/// If the OutputT object contains pointers to other objects created by JFactories, the user still has to make
/// sure that their respective JFactories aren't claiming ownership either.

template <typename InputT, typename OutputT>
class ErsapEventProcessor : public JEventProcessor {

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
