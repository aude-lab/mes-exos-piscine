#include "runner/smart_impl.hh"

#include <algorithm>
#include <cctype>
#include <libzork/exceptions.hh>
#include <yaml-cpp/yaml.h>

#include "exceptions.hh"
#include "story/node_impl.hh"

namespace libzork::runner
{

    SmartRunnerImpl::SmartRunnerImpl(std::unique_ptr<story::Story> story,
                                     const fs::path& synonyms_path,
                                     std::istream& is, std::ostream& os)
        : SmartRunner(std::move(story), is, os)
    {
        YAML::Node yaml_data = YAML::LoadFile(synonyms_path.string());
        for (const auto& entry : yaml_data)
        {
            std::string word = entry["word"].as<std::string>();
            known_words_.insert(word);
            std::unordered_set<std::string> syns;
            for (const auto& syn : entry["synonyms"])
            {
                std::string s = syn.as<std::string>();
                syns.insert(s);
                known_words_.insert(s);
            }
            synonyms_[word] = std::move(syns);
        }
    }

    std::unordered_set<std::string>
    SmartRunnerImpl::tokenize(const std::string& str) const
    {
        std::unordered_set<std::string> tokens;
        std::string current;
        for (char c : str)
        {
            if (std::isalnum(static_cast<unsigned char>(c)))
                current += static_cast<char>(
                    std::tolower(static_cast<unsigned char>(c)));
            else
            {
                if (!current.empty())
                {
                    if (known_words_.count(current))
                        tokens.insert(current);
                    current.clear();
                }
            }
        }
        if (!current.empty() && known_words_.count(current))
            tokens.insert(current);
        return tokens;
    }

    bool SmartRunnerImpl::has_unmatched_token(
        const std::unordered_set<std::string>& user_tokens,
        const std::unordered_set<std::string>& choice_tokens) const
    {
        for (const auto& ct : choice_tokens)
        {
            if (user_tokens.count(ct))
                continue;

            bool found = false;
            auto it = synonyms_.find(ct);
            if (it != synonyms_.end())
            {
                for (const auto& ut : user_tokens)
                {
                    if (it->second.count(ut))
                    {
                        found = true;
                        break;
                    }
                }
            }

            if (!found)
            {
                for (const auto& ut : user_tokens)
                {
                    auto uit = synonyms_.find(ut);
                    if (uit != synonyms_.end() && uit->second.count(ct))
                    {
                        found = true;
                        break;
                    }
                }
            }

            if (!found)
                return true;
        }
        return false;
    }

    void SmartRunnerImpl::handle_builtin(const std::string& cmd)
    {
        std::string lower_cmd = cmd.substr(1);
        std::string cmd_name;
        std::string cmd_arg;
        auto sp = lower_cmd.find(' ');
        if (sp == std::string::npos)
            cmd_name = lower_cmd;
        else
        {
            cmd_name = lower_cmd.substr(0, sp);
            cmd_arg = lower_cmd.substr(sp + 1);
        }
        for (char& c : cmd_name)
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

        if (cmd_name == "quit")
            throw libzork::RunnerQuit();

        if (cmd_name == "brief")
            return;

        if (cmd_name == "jump")
            throw libzork::RunnerInterrupt("Do you expect me to applaud?");

        if (cmd_name == "shout")
            throw libzork::RunnerInterrupt("Aaaarrrrgggghhhh!");

        if (cmd_name == "inventory")
        {
            const auto inventory =
                this->story_->get_store()->get_inventory();
            std::string msg;
            for (const auto& [name, value] : inventory)
                msg += name + ": " + std::to_string(value) + "\n";
            if (!msg.empty() && msg.back() == '\n')
                msg.pop_back();
            throw libzork::RunnerInterrupt(msg);
        }

        (void)cmd_arg;
        throw libzork::RunnerInterrupt("Unknown command: " + cmd);
    }

    void SmartRunnerImpl::process_input()
    {
        std::string input;
        std::getline(this->is_, input);

        if (!input.empty() && input[0] == '.')
        {
            handle_builtin(input);
            return;
        }

        auto user_tokens = tokenize(input);

        const auto* current = this->story_->get_current();
        const auto& node_impl = story::to_impl(*current);
        const auto all_choices = current->list_choices(false);
        const std::size_t total_raw = all_choices.size();

        for (std::size_t i = 0; i < total_raw; ++i)
        {
            if (!node_impl.check_conditions(i))
                continue;

            auto choice_tokens = tokenize(all_choices[i]);

            if (!has_unmatched_token(user_tokens, choice_tokens))
            {
                const auto* next = current->get_choice(i, true);
                if (next)
                {
                    this->story_->set_current(next);
                    return;
                }
            }
        }

        throw libzork::RunnerInterrupt("I beg your pardon?");
    }

} // namespace libzork::runner
