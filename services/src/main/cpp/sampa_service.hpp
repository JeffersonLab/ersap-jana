#ifndef ERSAP_JANA_SAMPA_SERVICE_HPP
#define ERSAP_JANA_SAMPA_SERVICE_HPP
#include <ersap/engine.hpp>

#include <atomic>
#include <memory>

namespace ersap {
    namespace jana {

        class SampaEngine;

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
            std::shared_ptr<SampaEngine> engine_{};
        };

    } // end namespace jana
} // end namespace ersap

#endif //ERSAP_JANA_SAMPA_SERVICE_HPP
