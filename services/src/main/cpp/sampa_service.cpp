
#include "sampa_service.hpp"

#include <sampa_data_type.hpp>
#include "ersap_event_processor.hpp"
#include "sampa_test_factory.hpp"
#include <ersap/stdlib/json_utils.hpp>

#include <JANA/JFactoryGenerator.h>

#include <iostream>
#include <ctime>

extern "C"
std::unique_ptr<ersap::Engine> create_engine()
{
    return std::make_unique<ersap::jana::SampaService>();
}

namespace ersap {
    namespace jana {

        time_t start;

        ersap::EngineData SampaService::configure(ersap::EngineData& input)
        {
            // Ersap provides a simple JSON parser to read configuration data
            // and configure the service.
            auto config = ersap::stdlib::parse_json(input);

            // TODO: Extract jana_config_file_name from clara config
            auto jana_config_file_name = "config.jana";

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
            m_evtsrc = new ErsapEventSource("ErsapEventSource", m_app);

            m_app->Add(m_evtsrc);
            m_app->Add(new JFactoryGeneratorT<SampaTestFactory>(""));
            m_app->Add(new ErsapEventProcessor<SampaDASMessage, SampaOutputMessage>());

            m_app->Run(false);  // Exit immediately, DON'T block until finished
            return {};
        }


        ersap::EngineData SampaService::execute(ersap::EngineData& input) {
            auto output = ersap::EngineData{};

//            time_t end = time(nullptr);
//             if (end - start >= 1) {
                // This always loads the shared_pointer into a new shared_ptr
                // std::atomic_load(&engine_)->process();
//                start = end;
//            }
            // Set and return output data
//            output.set_data(SAMPA_DAS, processed_data);

            // TODO: Implement me!
            return input;
        }

        ersap::EngineData SampaService::execute_group(const std::vector<ersap::EngineData>&)
        {
            return {};
        }

        std::vector<ersap::EngineDataType> SampaService::input_data_types() const
        {
            return { ersap::type::JSON, ersap::type::BYTES };
//            return { ersap::type::JSON, SAMPA_DAS };
        }


        std::vector<ersap::EngineDataType> SampaService::output_data_types() const
        {
            return { ersap::type::JSON, ersap::type::BYTES };
//            return { ersap::type::JSON, SAMPA_DAS };
        }


        std::set<std::string> SampaService::states() const
        {
            return std::set<std::string>{};
        }


        std::string SampaService::name() const
        {
            return "SampaService";
        }


        std::string SampaService::author() const
        {
            return "Vardan Gyurjyan";
        }


        std::string SampaService::description() const
        {
            return "Jana based sampa data processing service example";
        }


        std::string SampaService::version() const
        {
            return "0.1";
        }

    }
}

