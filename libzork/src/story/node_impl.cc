#include "story/node_impl.hh"

#include <fstream>

#include "exceptions.hh"

namespace libzork::story
{
	NodeImpl::NodeImpl(const std::string& name, const fs::path& script_path)
        : name_(name)
    {
        std::ifstream ifs(script_path);
        if (!ifs.is_open())
            throw std::invalid_argument("Could not open script file: "
                                        + script_path.string());

        std::string line;
        std::string text_content;
        while (std::getline(ifs, line))
        {
            text_content += line + "\n";
        }
        text_ = text_content;
    }

    const std::string& NodeImpl::get_name() const
    {
        return name_;
    }

    const std::string& NodeImpl::get_text() const
    {
        return text_;
    }

    bool NodeImpl::check_conditions(std::size_t index) const
    {
        if (index >= choices_.size())
            return false;
        for (const auto& condition : choices_[index].conditions)
        {
            if (!condition->apply())
                return false;
        }
        return true;
    }

    const Node* NodeImpl::get_choice(size_t index, bool check_conditions) const
    {
        if (index >= choices_.size())
            return nullptr;

        const auto& choice = choices_[index];

        if (check_conditions)
        {
            for (const auto& condition : choice.conditions)
            {
                if (!condition->apply())
                    return nullptr;
            }
            for (const auto& action : choice.actions)
                action->apply();
        }

        return choice.target;
    }

    std::vector<std::string> NodeImpl::list_choices(bool check_conditions) const
    {
        std::vector<std::string> result;
        for (const auto& choice : choices_)
        {
            if (check_conditions)
            {
                bool all_conditions_met = true;
                for (const auto& condition : choice.conditions)
                {
                    if (!condition->apply())
                    {
                        all_conditions_met = false;
                        break;
                    }
                }
                if (!all_conditions_met)
                    continue;
            }

            result.push_back(choice.text);
        }
        return result;
    }

    void NodeImpl::add_choice(
        const Node* other, const std::string& text,
        std::vector<std::unique_ptr<vars::Condition>> conditions,
        std::vector<std::unique_ptr<vars::Action>> actions)
    {
        choices_.push_back(
            Choice{ other, text, std::move(conditions), std::move(actions) });
    }

    const NodeImpl& to_impl(const Node& node)
    {
        return dynamic_cast<const NodeImpl&>(node);
    }

    NodeImpl& to_impl(Node& node)
    {
        return dynamic_cast<NodeImpl&>(node);
    }



} // namespace libzork::story
