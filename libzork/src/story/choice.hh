#pragma once

#include <libzork/vars/action.hh>
#include <libzork/vars/condition.hh>
#include <memory>
#include <vector>

namespace libzork::story
{
    struct Choice
    {
        const Node* target;
        std::string text;
        std::vector<std::unique_ptr<vars::Condition>> conditions;
        std::vector<std::unique_ptr<vars::Action>> actions;
    };
} // namespace libzork::story
