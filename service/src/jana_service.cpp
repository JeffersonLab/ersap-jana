//
// Created by Vardan Gyurjyan on 1/28/21.
//

#include "jana_service.hpp"
#include "group_event_processor.hpp"
#include <clara/stdlib/json_utils.hpp>

extern "C"
std::unique_ptr<clara::Engine> create_engine() {
    return std::make_unique<clara::jana::JanaService>();
}
namespace clara {
    namespace jana {

        clara::EngineData JanaService::configure(clara::EngineData &input) {
            // Clara provides a simple JSON parser to read configuration data
            // and configure the service.
            auto config = clara::stdlib::parse_json(input);
            // TODO: Extract jana_config_file_name from clara config
            auto jana_config_file_name = "config.jana";

            // Example for when the service has state that is configured by
            // the orchestrator. The "state" object should be a std::shared_ptr
            // accessed atomically.
            //
            // (This service is actually stateless, so engine_ could just simply be
            // initialized in the service constructor).
//            std::atomic_store(&engine_, std::make_shared<JanaEngine>());

            std::cout << "Initializing JApplication" << std::endl;fflush(stdout);
            // std::cout << "Loading options from " << jana_config_file_name << std::endl;fflush(stdout);
            // auto params = new JParameterManager;
            // try {
            //     params->ReadConfigFile(jana_config_file_name);
            // } catch (JException& e) {
            //     std::cout << "Problem reading config file '" << jana_config_file_name << "'. Exiting." << std::endl << std::endl;
            //     exit(-1);
            // }

            m_app = new JApplication(); // (params)
            m_evtsrc = new ClaraEventSource("ClaraEventSource", m_app);

            m_app->Add(m_evtsrc);
            m_app->Add(new GroupedEventProcessor()); // TODO: Move this into ClaraEventSource::Finish() hopefully
            // Add any event processors you might need

            m_app->Run(false);  // Exit immediately, DON'T block until finished
            return {};
        }

        clara::EngineData JanaService::execute(clara::EngineData &input) {

            // Cast to sampa?
            std::vector<SampaEvent*> group;
            group.push_back(any_cast<SampaEvent*>(input.data()));
            m_evtsrc->SubmitAndWait(group);

            // TODO: Extract output objects from JEvent and pass back to clara
            auto output = clara::EngineData{};
            return output;
        }

        clara::EngineData JanaService::execute_group(const std::vector<clara::EngineData> & untyped_group) {
            std::vector<SampaEvent*> typed_group;
            typed_group.reserve(untyped_group.size());
            for (auto item : untyped_group) {
                typed_group.push_back(any_cast<SampaEvent*>(item.data()));
            }
            m_evtsrc->SubmitAndWait(typed_group);

            // TODO: Extract output objects from JEvents and pass back to clara
            auto output = clara::EngineData{};
            return output;
        }


        std::vector<clara::EngineDataType> JanaService::input_data_types() const {
            return {clara::type::STRING};
        }


        std::vector<clara::EngineDataType> JanaService::output_data_types() const {
            return {clara::type::STRING};
        }


        std::set<std::string> JanaService::states() const {
            return std::set<std::string>{};
        }


        std::string JanaService::name() const {
            return "JanaService";
        }


        std::string JanaService::author() const {
            return "Nathan Brei";
        }


        std::string JanaService::description() const {
            return "Lets use JANA inside a CLARA data processing pipeline";
        }


        std::string JanaService::version() const {
            return "0.1";
        }
    }
}
