#ifndef SCIA_RESAMPLING_SERVICE_HPP
#define SCIA_RESAMPLING_SERVICE_HPP

#include <ersap/engine.hpp>

#include <atomic>
#include <memory>

namespace ersap {
namespace jana {

class JanaEngine;

class JanaService : public ersap::Engine
{
public:
    JanaService() = default;

    JanaService(const JanaService&) = delete;
    JanaService& operator=(const JanaService&) = delete;

    JanaService(JanaService&&) = default;
    JanaService& operator=(JanaService&&) = default;

    ~JanaService() override = default;

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
    std::shared_ptr<JanaEngine> engine_{};
};

} // end namespace jana
} // end namespace ersap

#endif
