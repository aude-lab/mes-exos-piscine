#include "runner/smart_impl.hh"

#include "exceptions.hh"

namespace libzork::runner
{

    void SmartRunnerImpl::process_input()
    {
        throw NotImplemented();
    }

    std::unordered_set<std::string>
    SmartRunnerImpl::tokenize(const std::string& str) const
    {
        (void)str;
        throw NotImplemented();
    }

    bool SmartRunnerImpl::has_unmatched_token(
        const std::unordered_set<std::string>& user_tokens,
        const std::unordered_set<std::string>& choice_tokens) const
    {
        (void)user_tokens;
        (void)choice_tokens;
        throw NotImplemented();
    }

} // namespace libzork::runner
