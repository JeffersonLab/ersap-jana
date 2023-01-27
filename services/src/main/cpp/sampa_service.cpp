
#include "sampa_service.hpp"

#include <sampa_data_type.hpp>
#include "ersap_event_processor.hpp"
#include "sampa_calib_factory.hpp"
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


            // TODO: Extract jana_config_file_name from ersap config
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
            m_evtsrc = new ErsapEventSource<SampaDASMessage, SampaDASMessage>("ErsapEventSource", m_app, "", "calibrated");

            m_app->Add(m_evtsrc);
            m_app->Add(new JFactoryGeneratorT<SampaCalibFactory>("calibrated"));
            m_app->Add(new ErsapEventProcessor<SampaDASMessage>("calibrated"));

            m_app->Run(false);  // Exit immediately, DON'T block until finished
            return {};
        }


        ersap::EngineData SampaService::execute(ersap::EngineData& input) {
            auto output = ersap::EngineData{};

            // If the mime-type is not supported, return an error.
            if (input.mime_type() != SAMPA_DAS) {
                output.set_status(ersap::EngineStatus::ERROR);
                output.set_description("Wrong input type");
                return output;
            }

            auto& sampa_input = ersap::data_cast<SampaDASMessage>(input);

            std::vector<SampaDASMessage*> sampa_inputs;
            sampa_inputs.push_back(&sampa_input);

            // TODO: Create an EventGroup optimized for exactly one event?
            // time_t end = time(nullptr);
            // if (end - start >= 1) {
            //     // This always loads the shared_pointer into a new shared_ptr
            //     std::atomic_load(&engine_)->process();
            //     start = end;
            // }
            auto sampa_outputs = m_evtsrc->SubmitAndWait(sampa_inputs);

            // Set and return output data
            output.set_data(SAMPA_DAS, sampa_outputs[0]);

            return output;
        }


        ersap::EngineData SampaService::execute_group(const std::vector<ersap::EngineData>& inputs)
        {
            auto output = ersap::EngineData{};
            std::vector<SampaDASMessage*> sampa_inputs;

            for (auto input : inputs) {

                // If the mime-type is not supported, return an error.
                if (input.mime_type() != SAMPA_DAS) {
                    output.set_status(ersap::EngineStatus::ERROR);
                    output.set_description("Wrong input type");
                    return output;
                }
                sampa_inputs.push_back(&ersap::data_cast<SampaDASMessage>(input));
            }

            // time_t end = time(nullptr);
            // if (end - start >= 1) {
            //     // This always loads the shared_pointer into a new shared_ptr
            //     std::atomic_load(&engine_)->process();
            //     start = end;
            // }

            auto sampa_outputs = m_evtsrc->SubmitAndWait(sampa_inputs);

            // Set and return output data
            output.set_data(SAMPA_DAS, sampa_outputs[0]);
            // TODO: Execute_group is many to one instead of many-to-many. I don't understand!

            return output;
        }

        std::vector<ersap::EngineDataType> SampaService::input_data_types() const
        {
            // TODO: Need to understand
            // return { ersap::type::JSON, ersap::type::BYTES };
            return { ersap::type::JSON, SAMPA_DAS };
        }


        std::vector<ersap::EngineDataType> SampaService::output_data_types() const
        {
            // TODO: Need to understand
            // return { ersap::type::JSON, ersap::type::BYTES };
            return { ersap::type::JSON, SAMPA_DAS };
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

