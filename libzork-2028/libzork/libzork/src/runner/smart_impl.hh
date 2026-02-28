#ifndef SMART_IMPL_HH
#define SMART_IMPL_HH

#include <libzork/runner/smart.hh>

namespace libzork::runner
{

    class SmartRunnerImpl : public SmartRunner
    {
    public:
        void process_input() override;
        virtual std::unordered_set<std::string>
        tokenize(const std::string& str) const override;
        virtual bool
        has_unmatched_token(const std::unordered_set<std::string>& user_tokens,
                            const std::unordered_set<std::string>&
                                choice_tokens) const override;
    };

} // namespace libzork::runner
#endif // !SMART_IMPL_HH
