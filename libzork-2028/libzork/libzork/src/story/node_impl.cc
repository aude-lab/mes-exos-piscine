#include "story/node_impl.hh"

#include <fstream>

#include "exceptions.hh"

namespace libzork::story
{
    const std::string& NodeImpl::get_name() const
    {
        return name_;
    }

    const std::string& NodeImpl::get_text() const
    {
        return text_;
    }

    bool NodeImpl::check_conditions_param(std::size_t index) const
    {
        if (index >= choices_.size())
            return false;

        for (const auto& cond : choices_[index].conditions)
        {
            if (!cond->apply())
                return false;
        }
        return true;
    }

    const Node* NodeImpl::get_choice(size_t index, bool check_conditions) const
    {
        if (index >= choices_.size())
            return nullptr;

        const auto& choice = choices_[index];

        if (check_conditions_param)
        {
            for (const auto& cond : choice.conditions)
            {
                if (!cond->apply())
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
        for (std::size_t i = 0; i < choices_.size(); ++i)
        {
            if (check_conditions && !check_conditions_param(i))
                continue;
            result.push_back(choices_[i].text);
        }
        return result;
    }

    void NodeImpl::add_choice(
        const Node* other, const std::string& text,
        std::vector<std::unique_ptr<vars::Condition>> conditions,
        std::vector<std::unique_ptr<vars::Action>> actions)
    {
        Choice c;
        c.target = other;
        c.text = text;
        c.conditions = std::move(conditions);
        c.actions = std::move(actions);
        choices_.push_back(std::move(c));
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
