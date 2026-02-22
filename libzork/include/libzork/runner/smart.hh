#pragma once

#include <libzork/runner/interactive.hh>
#include <unordered_set>

namespace libzork::runner
{

    class SmartRunner : public InteractiveRunner
    {
    public:
        using InteractiveRunner::InteractiveRunner;

        ~SmartRunner() override = default;

        virtual std::unordered_set<std::string>
        tokenize(const std::string& str) const = 0;
        virtual bool has_unmatched_token(
            const std::unordered_set<std::string>& user_tokens,
            const std::unordered_set<std::string>& choice_tokens) const = 0;
    };

    std::unique_ptr<SmartRunner> make_smart_runner(
        std::unique_ptr<story::Story> story, const fs::path& synonyms_path,
        std::istream& is = std::cin, std::ostream& os = std::cout);

} // namespace libzork::runner
