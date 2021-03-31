//
// Created by Vardan Gyurjyan on 1/28/21.
//

#ifndef ERSAP_JANA_JANA_SERVICE_HPP
#define ERSAP_JANA_JANA_SERVICE_HPP

#include <clara/engine.hpp>

#include <JANA/JApplication.h>
#include "clara_event_source.hpp"


namespace clara {
    namespace jana {


class JanaService : public clara::Engine
{
public:
    JanaService() = default;

    JanaService(const JanaService&) = delete;
    JanaService& operator=(const JanaService&) = delete;

    JanaService(JanaService&&) = default;
    JanaService& operator=(JanaService&&) = default;

    ~JanaService() override = default;

public:
    clara::EngineData configure(clara::EngineData&) override;

    clara::EngineData execute(clara::EngineData&) override;

    clara::EngineData execute_group(const std::vector<clara::EngineData>&) override;

public:
    std::vector<clara::EngineDataType> input_data_types() const override;

    std::vector<clara::EngineDataType> output_data_types() const override;

    std::set<std::string> states() const override;

public:
    std::string name() const override;

    std::string author() const override;

    std::string description() const override;

    std::string version() const override;

private:
    JApplication* m_app = nullptr;
    ClaraEventSource* m_evtsrc = nullptr;
};

    } // end namespace jana
} // end namespace clara


#endif //ERSAP_JANA_JANA_SERVICE_HPP
