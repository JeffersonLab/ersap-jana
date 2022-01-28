
#include <JANA/JFactoryT.h>
#include <numeric>


struct SampaOutputMessage {
    int sum = 0;
};


class SampaTestFactory : public JFactoryT<SampaOutputMessage> {
    std::string input_tag;
public:
    SampaTestFactory();
    void Init() override;
    void Process(const std::shared_ptr<const JEvent>& aEvent) override;
};


SampaTestFactory::SampaTestFactory() {
    SetFactoryFlag(NOT_OBJECT_OWNER);
}

void SampaTestFactory::Init()  {
    GetApplication()->GetParameter("factory_input_tag", input_tag);
}

void SampaTestFactory::Process(const std::shared_ptr<const JEvent> &event) {

    auto summary = new SampaOutputMessage();
    auto sampa_message = event->GetSingle<ersap::jana::SampaDASMessage>(input_tag);
    if (!sampa_message->payload.empty()) {
        summary->sum = std::accumulate(std::begin(sampa_message->payload[0]), std::end(sampa_message->payload[0]), 0);
    }
    LOG << "Setting sum=" << summary->sum << LOG_END;
    Insert(summary);

}

