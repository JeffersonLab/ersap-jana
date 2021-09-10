#ifndef ERSAP_JANA_SAMPA_SERVICE_HPP
#define ERSAP_JANA_SAMPA_SERVICE_HPP
#include <ersap/engine.hpp>

#include <atomic>
#include <memory>
#include <JANA/JApplication.h>
#include "ersap_event_source.hpp"
#include "sampa_data_type.hpp"
#include "sampa_output_type.hpp"

namespace ersap {
    namespace jana {

        // class SampaEngine;

        class SampaService : public ersap::Engine
        {
        public:
            SampaService() = default;

            SampaService(const SampaService&) = delete;
            SampaService& operator=(const SampaService&) = delete;

            SampaService(SampaService&&) = default;
            SampaService& operator=(SampaService&&) = default;

            ~SampaService() override = default;

        public:
            ersap::EngineData configure(ersap::EngineData&) override;

            ersap::EngineData execute(ersap::EngineData&) override;

            ersap::EngineData execute_group(const std::vector<ersap::EngineData>&) override;

        public:
            std::vector<ersap::EngineDataType> input_data_types() const override;

            std::vector<ersap::EngineDataType> output_data_types() const override;

            std::set<std::string> states() const override;

        public:
            std::string name() const override;

            std::string author() const override;

            std::string description() const override;

            std::string version() const override;

        private:
            // TODO: Use smart pointers here
            JApplication* m_app;
            ErsapEventSource<SampaDASMessage, SampaOutputMessage>* m_evtsrc;
            // std::shared_ptr<SampaEngine> engine_{};
        };

    } // end namespace jana
} // end namespace ersap

#endif //ERSAP_JANA_SAMPA_SERVICE_HPP
