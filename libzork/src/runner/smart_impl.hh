#pragma once

#include <libzork/runner/smart.hh>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace libzork::runner
{

    class SmartRunnerImpl : public SmartRunner
    {
    public:
        SmartRunnerImpl(std::unique_ptr<story::Story> story,
                        const fs::path& synonyms_path,
                        std::istream& is,
                        std::ostream& os);
        ~SmartRunnerImpl() override = default;

        void process_input() override;
        std::unordered_set<std::string>
        tokenize(const std::string& str) const override;
        bool has_unmatched_token(
            const std::unordered_set<std::string>& user_tokens,
            const std::unordered_set<std::string>& choice_tokens)
            const override;

    private:
        std::unordered_map<std::string, std::unordered_set<std::string>>
            synonyms_;
        std::unordered_set<std::string> known_words_;

        void handle_builtin(const std::string& cmd);
    };

} // namespace libzork::runner
