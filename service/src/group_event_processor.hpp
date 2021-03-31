//
// Created by Nathan Brei on 2019-12-15.
//

#ifndef JANA2_GROUPEDEVENTPROCESSOR_H
#define JANA2_GROUPEDEVENTPROCESSOR_H

#include <JANA/JEventProcessor.h>
#include <JANA/JLogger.h>
#include <JANA/Services/JEventGroupTracker.h>
#include <JANA/Utils/JPerfUtils.h>


/// GroupedEventProcessor demonstrates basic usage of JEventGroups

class GroupedEventProcessor : public JEventProcessor {
    std::mutex m_mutex;

public:
    void Process(const std::shared_ptr<const JEvent>& event) override;
};




#endif //JANA2_GROUPEDEVENTPROCESSOR_H
